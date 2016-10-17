#pragma once

#include "bitmap.h"

namespace bmp
{
	namespace px
	{
		// encapsulates access to pixel data through buffer iterator
		class Pixel;
		// iterator to RawData pixels
		class iterator;
		// returns beginning iterator to RawData
		inline iterator Begin(RawData &raw);
		// returns ending iterator to RawData
		inline iterator End(RawData &raw);

		class Pixel
		{
		public:
			friend class iterator;

			Pixel() : color(new RGB8()) {};

			Pixel(RawData &raw,buffIt_t pos_) : color(raw.ColorType()),pos(pos_) {};

			// returns the green component of the pixel at pos
			byte Red() const { return color->Red(pos); };
			// returns the green component of the pixel at pos
			byte Green() const { return color->Green(pos); };
			// returns the blue component of the pixel at pos
			byte Blue() const { return color->Blue(pos); };
			// returns the alpha component of the pixel at pos
			byte Alpha() const { return color->Alpha(pos); };
			// returns the brightness/gray component of the pixel at pos
			byte Brighteness() const { return color->Brighteness(pos); };

			// sets and returns the red component of the pixel at pos
			byte Red(byte v) const { return color->Red(pos,v); };
			// sets and returns the green component of the pixel at pos
			byte Green(byte v) const { return color->Green(pos,v); };
			// sets and returns the blue component of the pixel at pos
			byte Blue(byte v) const { return color->Blue(pos,v); };
			// sets and returns the alpha component of the pixel at pos
			byte Alpha(byte v) const { return color->Alpha(pos,v); };
			// sets and returns the brightness/gray component of the pixel at pos
			byte Brighteness(byte v) const { return color->Brighteness(pos,v); };

			// returns the green component of the pixel at pos
			float RedF() const { return color->RedF(pos); };
			// returns the green component of the pixel at pos
			float GreenF() const { return color->GreenF(pos); };
			// returns the blue component of the pixel at pos
			float BlueF() const { return color->BlueF(pos); };
			// returns the alpha component of the pixel at pos
			float AlphaF() const { return color->AlphaF(pos); };
			// returns the brightness/gray component of the pixel at pos
			float BrightenessF() const { return color->BrightenessF(pos); };

			// sets and returns the red component of the pixel at pos
			float RedF(float v) const { return color->RedF(pos,v); };
			// sets and returns the green component of the pixel at pos
			float GreenF(float v) const { return color->GreenF(pos,v); };
			// sets and returns the blue component of the pixel at pos
			float BlueF(float v) const { return color->BlueF(pos,v); };
			// sets and returns the alpha component of the pixel at pos
			float AlphaF(float v) const { return color->AlphaF(pos,v); };
			// sets and returns the brightness/gray component of the pixel at pos
			float BrightenessF(float v) const { return color->BrightenessF(pos,v); };
		private:
			buffIt_t		pos;// iterator into pixel buffer
			ColorTypePtr_T	color;// color format of pixel data
		};

		class iterator
		  : public boost::iterator_facade<
				iterator
				, Pixel
				, boost::random_access_traversal_tag >
		{
		 public:
			iterator() {};

			iterator(RawData &raw,buffIt_t pos) : pixel(raw,pos) {};

		 private:
			friend class boost::iterator_core_access;

			inline void increment() { advance(1); };

			inline void decrement() { advance(-1); };

			inline void advance(std::ptrdiff_t n) {	pixel.pos+=pixel.color->BytesPerPixel()*n;	};

			inline bool equal(iterator const& other) const { return pixel.pos == other.pixel.pos; };

			inline std::ptrdiff_t distance_to(iterator &it) { return std::distance(pixel.pos,it->pos)/pixel.color->BytesPerPixel(); };

			inline Pixel& dereference() const { return const_cast<Pixel&>(pixel); };

			Pixel pixel; // pixel data
		};

		inline iterator Begin(RawData &raw)
			{ return iterator(raw,raw.pixels.begin()); };

		inline iterator End(RawData &raw)
			{ return iterator(raw,raw.pixels.end()); };
	}
};