#include "stdafx.h"
#include "convertFilter.h"
#include "px.h"

using namespace bmp::filters;

void Convert::operator()(bmp::RawData &source,bmp::RawData &dest)
{
	// allocate space in destination
	dest.pixels.resize(colorPtr->BytesPerPixel()*source.Width()*source.Height());
	// copy description to destination
	dest.description=source.description;
	// set destination to conversion color type
	dest.description.color=colorPtr;
	// handle conversions between single channel types differently
	if ((dest.NChannels()==1)&&(source.NChannels()==1))
	{ // copy pixels
		dest.pixels=source.pixels;
		return;
	}
	// copy converted image from source to dest
	px::iterator begin=px::Begin(source);
	px::iterator end=px::End(source);
	px::iterator destBegin=px::Begin(dest);
	px::iterator destEnd=px::End(dest);
	if (source.BytesPerChannel()==1)
	{
		for (begin;begin!=end;++begin)
		{
			destBegin->Red(begin->Red());
			destBegin->Green(begin->Green());
			destBegin->Blue(begin->Blue());
			destBegin->Alpha(begin->Blue());
			++destBegin;
		}
	}
	else
	{
		for (begin;begin!=end;++begin)
		{
			destBegin->RedF(begin->RedF());
			destBegin->GreenF(begin->GreenF());
			destBegin->BlueF(begin->BlueF());
			destBegin->AlphaF(begin->AlphaF());
			++destBegin;
		}
	}
}
