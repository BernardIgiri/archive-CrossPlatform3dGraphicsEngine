#include "stdafx.h"
#include "bitmap.h"
#include "texture.h"
#include "filters.h"

namespace tex
{
	// filter for uploading bitmaps through opengl
	class UploadBitmap : public bmp::Filter
	{
	public:
		UploadBitmap(GLenum pxType_, bool genMipmaps_,bmp::Bitmap &bmap)
			: pxType(pxType_), genMipmaps(genMipmaps_) { bmap.ApplyFilter(*this); };
		void operator()(bmp::RawData &raw)
		{
		    size_t sz=raw.pixels.size();
			SetEnv(genMipmaps);
			if (genMipmaps) // generate mipmaps
				gluBuild2DMipmaps(GL_TEXTURE_2D, raw.NChannels(), raw.Width(),
					raw.Height(), pxType, GL_UNSIGNED_BYTE, &raw.pixels.front());
			else // upload texture as mipmap level 0
				UploadMipmapLvl(pxType,0,raw);
		};

		GLenum pxType; // pixel type
		bool genMipmaps; // true if mipmaps should be used
	};
};

using namespace tex;

GLenum tex::PixelType(const bmp::ColorType &colorType)
{
	if (typeid(colorType)==typeid(bmp::RGB8))
		return GL_RGB;
	else if (typeid(colorType)==typeid(bmp::RGBA8))
		return GL_RGBA;
	else if (typeid(colorType)==typeid(bmp::BA8))
		return GL_LUMINANCE_ALPHA;
	else if (typeid(colorType)==typeid(bmp::GRAY8))
		return GL_LUMINANCE;
	else if (typeid(colorType)==typeid(bmp::ALPHA8))
		return GL_ALPHA;
	else if (typeid(colorType)==typeid(bmp::RGB16))
		return GL_RGB;
	else if (typeid(colorType)==typeid(bmp::RGBA16))
		return GL_RGBA;
	else if (typeid(colorType)==typeid(bmp::BA16))
		return GL_LUMINANCE_ALPHA;
	else if (typeid(colorType)==typeid(bmp::GRAY16))
		return GL_LUMINANCE;
	else if (typeid(colorType)==typeid(bmp::ALPHA16))
		return GL_ALPHA;
	return GL_RGB;
}

void tex::SetEnv(bool isMipmapped)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (isMipmapped ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR));
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Texture::Upload(bmp::Bitmap &bmap, bool genMipmaps)
{
	Bind();
	UploadBitmap uploadTexture(PixelType(*bmap.ColorType()),genMipmaps,bmap);
}

void Texture::CopyScreen(unsigned x,unsigned y,unsigned width,unsigned height,bmp::ColorTypePtr_T color)
{
	bmp::Bitmap bmap;
	bmp::filters::BlankBitmap(bmap,width,height,color);
	Upload(bmap,false);
	Bind();
	glCopyTexImage2D(GL_TEXTURE_2D, 0, PixelType(*bmap.ColorType()), 0, 0, 128, 128, 0);
}
