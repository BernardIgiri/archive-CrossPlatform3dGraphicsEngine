#pragma once
#include "bitmap.h"

namespace tex
{
	// returns the opengl pixel type from the given color type
	GLenum PixelType(const bmp::ColorType &colorType);
	// sets the opengl enviroment vars for uploading a texture
	void SetEnv(bool isMipmapped);

	// texture object used to upload and bind textures
	class Texture : boost::noncopyable
	{
	public:
		// creates a default texture object
		Texture() : id(GL_INVALID_VALUE) { glGenTextures(1,&id); };
		// creates a texture with bitmap bmap
		Texture(bmp::Bitmap &bmap,bool genMipmaps) : id(GL_INVALID_VALUE) { glGenTextures(1,&id); Upload(bmap,genMipmaps); };
		// creates a mipmapped texture from bitmap begin to end
		template <typename InputIterator>
		Texture(InputIterator begin,InputIterator end) : id(GL_INVALID_VALUE) { glGenTextures(1,&id); UploadMipmap(begin,end); };
		// releases texture resources
		~Texture(void) { glDeleteTextures(1,&id); };
		// equality operator
		bool operator==(const Texture& tex) const { return id==tex.id; };
		// comparison less than operator
		bool operator<(const Texture& tex) const { return id<tex.id; };
		// comparison greater than operator
		bool operator>(const Texture& tex) const { return id>tex.id; };
		// binds texture as current texture
		void Bind() const { glBindTexture(GL_TEXTURE_2D,id); };
		// Copy screen to texture
		void CopyScreen(unsigned x,unsigned y,unsigned width,unsigned height,bmp::ColorTypePtr_T color);
		// uploads bitmap to texture memory, if genMipMaps is true, then a mip mapping is generated
		void Upload(bmp::Bitmap &bmap,bool genMipmaps);
		// uploads a mipmap of bitmap begin to end to texture memory
		template <typename InputIterator>
		void UploadMipmap(InputIterator begin,InputIterator end);
	private:
		GLuint id; // opengl texture id
	};

	// uploads 1 mipmap level through opengl
	class UploadMipmapLvl : public bmp::Filter
	{
	public:
		UploadMipmapLvl(GLenum pxType_,unsigned short idx_,bmp::Bitmap &bmap)
			: pxType(pxType_), idx(idx_) { bmap.ApplyFilter(*this);  };
		UploadMipmapLvl(GLenum pxType_,unsigned short idx_,bmp::RawData &raw)
			: pxType(pxType_), idx(idx_) { operator()(raw);  };
		inline void operator()(bmp::RawData &raw)
		{
			glTexImage2D(GL_TEXTURE_2D,idx,raw.NChannels(),raw.Width(),raw.Height(),
				0,pxType,GL_UNSIGNED_BYTE,&raw.pixels.front());
		};

		GLenum pxType; // pixel type
		unsigned short idx; // mip map index
	};

	template <typename InputIterator>
	void Texture::UploadMipmap(InputIterator begin, InputIterator end)
	{
		SetEnv(true);
		int idx=0; // start at index 0
		for (begin;begin!=end;++begin)
			UploadMipmapLvl(PixelType(*begin->ColorType()),idx++,*begin);
	};
};
