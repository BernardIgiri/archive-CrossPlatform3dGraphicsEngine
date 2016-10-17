#include "stdafx.h"
#include "filters.h"
#include "px.h"

using namespace bmp::filters;

void Invert::operator()(bmp::RawData &raw)
{
	px::iterator begin=px::Begin(raw);
	px::iterator end=px::End(raw);
	if (raw.BytesPerChannel()==1)
	{
		for (begin;begin!=end;++begin)
		{
			begin->Red(255-begin->Red());
			begin->Green(255-begin->Green());
			begin->Blue(255-begin->Blue());
		}
	}
	else
	{
		for (begin;begin!=end;++begin)
		{
			begin->RedF(1.0f-begin->Red());
			begin->GreenF(1.0f-begin->Green());
			begin->BlueF(1.0f-begin->Blue());
		}
	}
}

void MultAlpha::operator()(bmp::RawData &raw)
{
	px::iterator begin=px::Begin(raw);
	px::iterator end=px::End(raw);
	if (raw.BytesPerChannel()==1)
	{
		for (begin;begin!=end;++begin)
		{
			begin->Red( (begin->Red()*static_cast<int>(begin->Alpha()) )/255);
			begin->Green( (begin->Green()*static_cast<int>(begin->Alpha()) )/255);
			begin->Blue( (begin->Blue()*static_cast<int>(begin->Alpha()) )/255);
		}
	}
	else
	{
		for (begin;begin!=end;++begin)
		{
			begin->RedF( begin->RedF()*begin->Alpha() );
			begin->GreenF( begin->GreenF()*begin->Alpha() );
			begin->BlueF( begin->BlueF()*begin->Alpha() );
		}
	}
}

void BlankBitmap::operator ()(bmp::RawData &raw)
{
	raw.description.width=width;
	raw.description.height=height;
	raw.description.color=color;
	raw.pixels.clear();
	raw.pixels.resize(raw.Size());
}
