#pragma once

namespace bmp
{
	// bitmap object provides an encapsulated inteface to RawData
	class Bitmap;
	// RAW bitmap data object contains: ImageDescription and raw pixel data
	class RawData;
	// bitmap description object contains: width,height,size,color format, etc... but no pixel data
	class ImageDescription;
	// Base class for bitmap filters that perform operations on bitmap data
	class Filter;
	// Base class for bitmap filters that perform operations on two bitmaps at a time
	class MergeFilter;
	// Base class for color formats
	class ColorType;

	// typedef for pixel buffer
	typedef std::vector<byte> PixelBuffer_t;
	// typedef for pixel buffer iterator
	typedef PixelBuffer_t::iterator buffIt_t;
	// pointer to color types
	typedef boost::shared_ptr<ColorType> ColorTypePtr_T;

	class ColorType
	{
	public:
		virtual ~ColorType() = 0;
		// returns the green component of the pixel at pos
		virtual byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		virtual byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		virtual byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		virtual byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		virtual byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		virtual byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		virtual byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		virtual byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		virtual byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		virtual byte Brighteness(buffIt_t pos,byte v) const;

		// returns the green component of the pixel at pos
		virtual float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		virtual float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		virtual float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		virtual float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		virtual float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		virtual float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		virtual float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		virtual float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		virtual float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		virtual float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		virtual unsigned NChannels() const = 0;
		// returns the number of bytes per pixel
		virtual unsigned BytesPerChannel() const = 0;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const { return BytesPerChannel()*NChannels(); };
		// returns true if alpha channel is present
		virtual bool IsAlpha() const = 0;
	};

	// 8 bit/channel RGB
	class RGB8 : public ColorType
	{
	public:
		~RGB8() {};
		// returns the green component of the pixel at pos
		byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos,byte v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 8 bit/channel RGBA
	class RGBA8 : public ColorType
	{
	public:
		~RGBA8() {};
		// returns the green component of the pixel at pos
		byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos,byte v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 8 bit/channel Brighteness/Alpha
	class BA8 : public ColorType
	{
	public:
		~BA8() {};
		// returns the green component of the pixel at pos
		byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos,byte v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 8 bit/channel Grayscale
	class GRAY8 : public ColorType
	{
	public:
		~GRAY8() {};
		// returns the green component of the pixel at pos
		byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos,byte v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 8 bit/channel Alpha Map
	class ALPHA8 : public ColorType
	{
	public:
		~ALPHA8() {};
		// returns the green component of the pixel at pos
		byte Red(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		byte Green(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		byte Red(buffIt_t pos,byte v) const;
		// sets and returns the green component of the pixel at pos
		byte Green(buffIt_t pos,byte v) const;
		// sets and returns the blue component of the pixel at pos
		byte Blue(buffIt_t pos,byte v) const;
		// sets and returns the alpha component of the pixel at pos
		byte Alpha(buffIt_t pos,byte v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		byte Brighteness(buffIt_t pos,byte v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};

	// 16 bit/channel RGB
	class RGB16 : public ColorType
	{
	public:
		~RGB16() {};
		// returns the green component of the pixel at pos
		float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 16 bit/channel RGBA
	class RGBA16 : public ColorType
	{
	public:
		~RGBA16() {};
		// returns the green component of the pixel at pos
		float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 16 bit/channel Brighteness/Alpha
	class BA16 : public ColorType
	{
	public:
		~BA16() {};
		// returns the green component of the pixel at pos
		float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 16 bit/channel Grayscale
	class GRAY16 : public ColorType
	{
	public:
		~GRAY16() {};
		// returns the green component of the pixel at pos
		float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};
	// 16 bit/channel Alpha Map
	class ALPHA16 : public ColorType
	{
	public:
		~ALPHA16() {};
		// returns the green component of the pixel at pos
		float RedF(buffIt_t pos) const;
		// returns the green component of the pixel at pos
		float GreenF(buffIt_t pos) const;
		// returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos) const;
		// returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos) const;
		// returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos) const;

		// sets and returns the red component of the pixel at pos
		float RedF(buffIt_t pos,float v) const;
		// sets and returns the green component of the pixel at pos
		float GreenF(buffIt_t pos,float v) const;
		// sets and returns the blue component of the pixel at pos
		float BlueF(buffIt_t pos,float v) const;
		// sets and returns the alpha component of the pixel at pos
		float AlphaF(buffIt_t pos,float v) const;
		// sets and returns the brightness/gray component of the pixel at pos
		float BrightenessF(buffIt_t pos,float v) const;

		// returns the number of channels
		unsigned NChannels() const;
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const;
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const;
		// returns true if alpha channel is present
		bool IsAlpha() const;
	};

	class Filter
	{
	public:
		virtual void operator()(RawData &raw) = 0;
		virtual ~Filter() = 0;
	};
	class MergeFilter
	{
	public:
		virtual void operator()(RawData &source,RawData &dest) = 0;
		virtual ~MergeFilter() = 0;
	};

	class ImageDescription
	{
	public:
		ImageDescription() : width(0),height(0),color(new RGB8()) {};
		ImageDescription(unsigned width_,unsigned height_,ColorTypePtr_T color_) :
			width(width_),height(height_),color(color_) {};
		ImageDescription(const ImageDescription& i) : width(i.width), height(i.height), color(i.color) { };
		ImageDescription &operator=(const ImageDescription& i)
			{ width=i.width;height=i.height;color=i.color;return *this; };
		// returns the color format of the bitmap
		ColorTypePtr_T ColorType() const { return color; };
		// returns the height of the bitmap
		unsigned Height() const { return height; };
		// returns the width of the bitmap
		unsigned Width() const { return width; };
		// returns the number of channels
		unsigned NChannels() const { return color->NChannels(); };
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const { return color->BytesPerChannel(); };
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const { return color->BytesPerPixel(); };
		// returns true if alpha channel is present
		bool IsAlpha() const { return color->IsAlpha(); };
		// returns the size of the bitmap data
		unsigned Size() const { return BytesPerPixel()*Width()*Height(); };

		unsigned width; // width in pixels
		unsigned height; // height in pixels
		ColorTypePtr_T color; // color format
	};

	class RawData
	{
	public:
		RawData() {};
		RawData(const ImageDescription &description_,const PixelBuffer_t &pixels_) :
			description(description_),pixels(pixels_) {};
		RawData(const RawData& r) : description(r.description), pixels(r.pixels) { };
		RawData &operator=(const RawData& r)
			{ description=r.description;pixels=r.pixels;return *this; };

		// returns the color format of the bitmap
		ColorTypePtr_T ColorType() const { return description.ColorType(); };
		// returns the height of the bitmap
		unsigned Height() const { return description.Height(); };
		// returns the width of the bitmap
		unsigned Width() const { return description.Width(); };
		// returns the number of channels
		unsigned NChannels() const { return description.NChannels(); };
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const { return description.BytesPerChannel(); };
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const { return description.BytesPerPixel(); };
		// returns true if alpha channel is present
		bool IsAlpha() const { return description.IsAlpha(); };
		// returns the size of the bitmap data
		unsigned Size() const { return description.Size(); };

		ImageDescription description; // image description object
		PixelBuffer_t pixels; // raw pixel data buffer
	};

	class Bitmap
	{
	public:
		// creates an empty Bitmap object
		Bitmap() {};
		// copy constructor
		Bitmap(Bitmap &b) : raw(b.raw) {};
		// assingment operator
		Bitmap &operator=(const Bitmap& b) { raw=b.raw;return *this; };
		// returns the color format of the bitmap
		ColorTypePtr_T ColorType() const { return raw.ColorType(); };
		// returns the height of the bitmap
		unsigned Height() const { return raw.Height(); };
		// returns the width of the bitmap
		unsigned Width() const { return raw.Width(); };
		// returns the number of channels
		unsigned NChannels() const { return raw.NChannels(); };
		// returns the number of bytes per pixel
		unsigned BytesPerChannel() const { return raw.BytesPerChannel(); };
		// returns the number of bytes per pixel
		unsigned BytesPerPixel() const { return raw.BytesPerPixel(); };
		// returns true if alpha channel is present
		bool IsAlpha() const { return raw.IsAlpha(); };
		// returns the size of the bitmap data
		unsigned Size() const { return raw.Size(); };
		// applies filter to bitmap
		void ApplyFilter(Filter &filter) { filter(raw); };
		// applies merge filter to bitmap
		void ApplyMergeFilter(MergeFilter &filter,Bitmap &source) { filter(source.raw,raw); };
	private:
		RawData raw; // raw bitmap data object
	};
};
