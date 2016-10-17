#pragma once
#include "bitmap.h"

namespace bmp
{
	namespace filters
	{
		// Inverts the color contents of bmap
		class Invert : public Filter
		{
		public:
			void operator()(bmp::RawData &raw);
		};
		// multiplies color with alpha
		class MultAlpha : public Filter
		{
		public:
			void operator()(bmp::RawData &raw);
		};
		// creates a blank bitmap with the given attributes
		class BlankBitmap : public bmp::Filter
		{
		public:
			BlankBitmap(Bitmap &bmap,unsigned width_,unsigned height_,ColorTypePtr_T color_)
				: width(width_), height(height_), color(color_) { bmap.ApplyFilter(*this); };
			void operator()(RawData &raw);
		private:
			unsigned width,height;
			ColorTypePtr_T color;
		};
	};
}
