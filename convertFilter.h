#pragma once
#include "bitmap.h"

namespace bmp
{
	namespace filters
	{
		// overwrites source bitmap, with dest bitmap after converting it to the specified color type
		class Convert : public MergeFilter
		{
		public:
			Convert() {};
			Convert(ColorTypePtr_T colorPtr_) : colorPtr(colorPtr_) {};
			// sets the color type to convert to
			void SetColorType(ColorTypePtr_T colorPtr_) { colorPtr = colorPtr_; };
			void operator()(RawData &source,RawData &dest);
		private:
			ColorTypePtr_T colorPtr; // pointer to color type
		};
	};
}