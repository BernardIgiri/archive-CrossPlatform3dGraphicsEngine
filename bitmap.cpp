#include "stdafx.h"
#include "bitmap.h"

using namespace bmp;

Filter::~Filter() {};
MergeFilter::~MergeFilter() {};
ColorType::~ColorType() {};

float GetFloat(bmp::buffIt_t pos) { return (*reinterpret_cast<boost::uint16_t*>(&(*pos)))/65535.0f; };
float SetFloat(bmp::buffIt_t pos,float v) { return *reinterpret_cast<boost::uint16_t*>(&(*pos))=static_cast<boost::uint16_t>(v*65535.0f); };

float bmp::ColorType::RedF(bmp::buffIt_t pos) const
{
	return Red(pos)/255.0f;
}
float bmp::ColorType::GreenF(bmp::buffIt_t pos) const
{
	return Green(pos)/255.0f;
}
float bmp::ColorType::BlueF(bmp::buffIt_t pos) const
{
	return Blue(pos)/255.0f;
}
float bmp::ColorType::AlphaF(bmp::buffIt_t pos) const
{
	return Alpha(pos)/255.0f;
}
float bmp::ColorType::BrightenessF(bmp::buffIt_t pos) const
{
	return Brighteness(pos)/255.0f;
}
float bmp::ColorType::RedF(bmp::buffIt_t pos,float v) const
{
	return Red(pos,static_cast<byte>(v*255.0f));
	return v;
}
float bmp::ColorType::GreenF(bmp::buffIt_t pos,float v) const
{
	return Green(pos,static_cast<byte>(v*255.0f));
	return v;
}
float bmp::ColorType::BlueF(bmp::buffIt_t pos,float v) const
{
	return Blue(pos,static_cast<byte>(v*255.0f));
	return v;
}
float bmp::ColorType::AlphaF(bmp::buffIt_t pos,float v) const
{
	return Alpha(pos,static_cast<byte>(v*255.0f));
	return v;
}
float bmp::ColorType::BrightenessF(bmp::buffIt_t pos,float v) const
{
	Brighteness(pos,static_cast<byte>(v*255.0f));
	return v;
}

byte bmp::ColorType::Red(bmp::buffIt_t pos) const
{
	return static_cast<byte>(RedF(pos)*255.0f);
}
byte bmp::ColorType::Green(bmp::buffIt_t pos) const
{
	return static_cast<byte>(GreenF(pos)*255.0f);
}
byte bmp::ColorType::Blue(bmp::buffIt_t pos) const
{
	return static_cast<byte>(BlueF(pos)*255.0f);
}
byte bmp::ColorType::Alpha(bmp::buffIt_t pos) const
{
	return static_cast<byte>(AlphaF(pos)*255.0f);
}
byte bmp::ColorType::Brighteness(bmp::buffIt_t pos) const
{
	return static_cast<byte>(BrightenessF(pos)*255.0f);
}
byte bmp::ColorType::Red(bmp::buffIt_t pos,byte v) const
{
	RedF(pos,v/255.0f);
	return v;
}
byte bmp::ColorType::Green(bmp::buffIt_t pos,byte v) const
{
	GreenF(pos,v/255.0f);
	return v;
}
byte bmp::ColorType::Blue(bmp::buffIt_t pos,byte v) const
{
	BlueF(pos,v/255.0f);
	return v;
}
byte bmp::ColorType::Alpha(bmp::buffIt_t pos,byte v) const
{
	AlphaF(pos,v/255.0f);
	return v;
}
byte bmp::ColorType::Brighteness(bmp::buffIt_t pos,byte v) const
{
	BrightenessF(pos,v/255.0f);
	return v;
}
//*************************
byte bmp::RGB8::Red(bmp::buffIt_t pos) const
{
	return *pos;
}
byte bmp::RGB8::Green(bmp::buffIt_t pos) const
{
	return *(pos+1);
}
byte bmp::RGB8::Blue(bmp::buffIt_t pos) const
{
	return *(pos+2);
}
byte bmp::RGB8::Alpha(bmp::buffIt_t pos) const
{
	return 255;
}
byte bmp::RGB8::Brighteness(bmp::buffIt_t pos) const
{
	return (Red(pos)+Green(pos)+Blue(pos))/3;
}
byte bmp::RGB8::Red(bmp::buffIt_t pos,byte v) const
{
	return *pos=v;
}
byte bmp::RGB8::Green(bmp::buffIt_t pos,byte v) const
{
	return *(pos+1)=v;
}
byte bmp::RGB8::Blue(bmp::buffIt_t pos,byte v) const
{
	return *(pos+2)=v;
}
byte bmp::RGB8::Alpha(bmp::buffIt_t pos,byte v) const
{
	return 255;
}
byte bmp::RGB8::Brighteness(bmp::buffIt_t pos,byte v) const
{
	Red(pos,v);Green(pos,v);Blue(pos,v);
	return v;
}
unsigned bmp::RGB8::NChannels() const
{
	return 3;
}
unsigned bmp::RGB8::BytesPerChannel() const
{
	return 1;
}
bool bmp::RGB8::IsAlpha() const
{
	return false;
}
//*************************
byte bmp::RGBA8::Red(bmp::buffIt_t pos) const
{
	return *pos;
}
byte bmp::RGBA8::Green(bmp::buffIt_t pos) const
{
	return *(pos+1);
}
byte bmp::RGBA8::Blue(bmp::buffIt_t pos) const
{
	return *(pos+2);
}
byte bmp::RGBA8::Alpha(bmp::buffIt_t pos) const
{
	return *(pos+3);
}
byte bmp::RGBA8::Brighteness(bmp::buffIt_t pos) const
{
	return (Red(pos)+Green(pos)+Blue(pos))/3;
}
byte bmp::RGBA8::Red(bmp::buffIt_t pos,byte v) const
{
	return *pos=v;
}
byte bmp::RGBA8::Green(bmp::buffIt_t pos,byte v) const
{
	return *(pos+1)=v;
}
byte bmp::RGBA8::Blue(bmp::buffIt_t pos,byte v) const
{
	return *(pos+2)=v;
}
byte bmp::RGBA8::Alpha(bmp::buffIt_t pos,byte v) const
{
	return *(pos+3)=v;
}
byte bmp::RGBA8::Brighteness(bmp::buffIt_t pos,byte v) const
{
	Red(pos,v);Green(pos,v);Blue(pos,v);
	return v;
}
unsigned bmp::RGBA8::NChannels() const
{
	return 4;
}
unsigned bmp::RGBA8::BytesPerChannel() const
{
	return 1;
}
bool bmp::RGBA8::IsAlpha() const
{
	return true;
}
//************************
byte bmp::BA8::Red(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::BA8::Green(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::BA8::Blue(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::BA8::Alpha(bmp::buffIt_t pos) const
{
	return *(pos+1);
}
byte bmp::BA8::Brighteness(bmp::buffIt_t pos) const
{
	return *pos;
}
byte bmp::BA8::Red(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::BA8::Green(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::BA8::Blue(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::BA8::Alpha(bmp::buffIt_t pos,byte v) const
{
	return *(pos+1)=v;
}
byte bmp::BA8::Brighteness(bmp::buffIt_t pos,byte v) const
{
	return *pos=v;
}
unsigned bmp::BA8::NChannels() const
{
	return 2;
}
unsigned bmp::BA8::BytesPerChannel() const
{
	return 1;
}
bool bmp::BA8::IsAlpha() const
{
	return true;
}
//************************
byte bmp::GRAY8::Red(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::GRAY8::Green(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::GRAY8::Blue(bmp::buffIt_t pos) const
{
	return Brighteness(pos);
}
byte bmp::GRAY8::Alpha(bmp::buffIt_t pos) const
{
	return 255;
}
byte bmp::GRAY8::Brighteness(bmp::buffIt_t pos) const
{
	return *pos;
}
byte bmp::GRAY8::Red(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::GRAY8::Green(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::GRAY8::Blue(bmp::buffIt_t pos,byte v) const
{
	return Brighteness(pos,v);
}
byte bmp::GRAY8::Alpha(bmp::buffIt_t pos,byte v) const
{
	return 255;
}
byte bmp::GRAY8::Brighteness(bmp::buffIt_t pos,byte v) const
{
	return *pos=v;
}
unsigned bmp::GRAY8::NChannels() const
{
	return 1;
}
unsigned bmp::GRAY8::BytesPerChannel() const
{
	return 1;
}
bool bmp::GRAY8::IsAlpha() const
{
	return false;
}
//*************************
byte bmp::ALPHA8::Red(bmp::buffIt_t pos) const
{
	return 0;
}
byte bmp::ALPHA8::Green(bmp::buffIt_t pos) const
{
	return 0;
}
byte bmp::ALPHA8::Blue(bmp::buffIt_t pos) const
{
	return 0;
}
byte bmp::ALPHA8::Alpha(bmp::buffIt_t pos) const
{
	return *pos;
}
byte bmp::ALPHA8::Brighteness(bmp::buffIt_t pos) const
{
	return 0;
}
byte bmp::ALPHA8::Red(bmp::buffIt_t pos,byte v) const
{
	return 0;
}
byte bmp::ALPHA8::Green(bmp::buffIt_t pos,byte v) const
{
	return 0;
}
byte bmp::ALPHA8::Blue(bmp::buffIt_t pos,byte v) const
{
	return 0;
}
byte bmp::ALPHA8::Alpha(bmp::buffIt_t pos,byte v) const
{
	return *pos=v;
}
byte bmp::ALPHA8::Brighteness(bmp::buffIt_t pos,byte v) const
{
	return 0;
}
unsigned bmp::ALPHA8::NChannels() const
{
	return 1;
}
unsigned bmp::ALPHA8::BytesPerChannel() const
{
	return 1;
}
bool bmp::ALPHA8::IsAlpha() const
{
	return true;
}
//*************************
float bmp::RGB16::RedF(bmp::buffIt_t pos) const
{
	return GetFloat(pos);
}
float bmp::RGB16::GreenF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+1);
}
float bmp::RGB16::BlueF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+2);
}
float bmp::RGB16::AlphaF(bmp::buffIt_t pos) const
{
	return 1.0f;
}
float bmp::RGB16::BrightenessF(bmp::buffIt_t pos) const
{
	return (RedF(pos)+GreenF(pos)+BlueF(pos))/3;
}
float bmp::RGB16::RedF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos,v);
}
float bmp::RGB16::GreenF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+1,v);
}
float bmp::RGB16::BlueF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+2,v);
}
float bmp::RGB16::AlphaF(bmp::buffIt_t pos,float v) const
{
	return 1.0f;
}
float bmp::RGB16::BrightenessF(bmp::buffIt_t pos,float v) const
{
	RedF(pos,v);GreenF(pos,v);BlueF(pos,v);
	return v;
}
unsigned bmp::RGB16::NChannels() const
{
	return 3;
}
unsigned bmp::RGB16::BytesPerChannel() const
{
	return 2;
}
bool bmp::RGB16::IsAlpha() const
{
	return false;
}
//*************************
float bmp::RGBA16::RedF(bmp::buffIt_t pos) const
{
	return GetFloat(pos);
}
float bmp::RGBA16::GreenF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+1);
}
float bmp::RGBA16::BlueF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+2);
}
float bmp::RGBA16::AlphaF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+3);
}
float bmp::RGBA16::BrightenessF(bmp::buffIt_t pos) const
{
	return (RedF(pos)+GreenF(pos)+BlueF(pos))/3;
}
float bmp::RGBA16::RedF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos,v);
}
float bmp::RGBA16::GreenF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+1,v);
}
float bmp::RGBA16::BlueF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+2,v);
}
float bmp::RGBA16::AlphaF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+3,v);
}
float bmp::RGBA16::BrightenessF(bmp::buffIt_t pos,float v) const
{
	RedF(pos,v);GreenF(pos,v);BlueF(pos,v);
	return v;
}
unsigned bmp::RGBA16::NChannels() const
{
	return 4;
}
unsigned bmp::RGBA16::BytesPerChannel() const
{
	return 2;
}
bool bmp::RGBA16::IsAlpha() const
{
	return true;
}
//************************
float bmp::BA16::RedF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::BA16::GreenF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::BA16::BlueF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::BA16::AlphaF(bmp::buffIt_t pos) const
{
	return GetFloat(pos+1);
}
float bmp::BA16::BrightenessF(bmp::buffIt_t pos) const
{
	return GetFloat(pos);
}
float bmp::BA16::RedF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::BA16::GreenF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::BA16::BlueF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::BA16::AlphaF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos+1,v);
}
float bmp::BA16::BrightenessF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos,v);
}
unsigned bmp::BA16::NChannels() const
{
	return 2;
}
unsigned bmp::BA16::BytesPerChannel() const
{
	return 2;
}
bool bmp::BA16::IsAlpha() const
{
	return true;
}
//************************
float bmp::GRAY16::RedF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::GRAY16::GreenF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::GRAY16::BlueF(bmp::buffIt_t pos) const
{
	return BrightenessF(pos);
}
float bmp::GRAY16::AlphaF(bmp::buffIt_t pos) const
{
	return 1.0f;
}
float bmp::GRAY16::BrightenessF(bmp::buffIt_t pos) const
{
	return GetFloat(pos);
}
float bmp::GRAY16::RedF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::GRAY16::GreenF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::GRAY16::BlueF(bmp::buffIt_t pos,float v) const
{
	return BrightenessF(pos,v);
}
float bmp::GRAY16::AlphaF(bmp::buffIt_t pos,float v) const
{
	return 1.0f;
}
float bmp::GRAY16::BrightenessF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos,v);
}
unsigned bmp::GRAY16::NChannels() const
{
	return 1;
}
unsigned bmp::GRAY16::BytesPerChannel() const
{
	return 2;
}
bool bmp::GRAY16::IsAlpha() const
{
	return false;
}
//*************************
float bmp::ALPHA16::RedF(bmp::buffIt_t pos) const
{
	return 0;
}
float bmp::ALPHA16::GreenF(bmp::buffIt_t pos) const
{
	return 0;
}
float bmp::ALPHA16::BlueF(bmp::buffIt_t pos) const
{
	return 0;
}
float bmp::ALPHA16::AlphaF(bmp::buffIt_t pos) const
{
	return GetFloat(pos);
}
float bmp::ALPHA16::BrightenessF(bmp::buffIt_t pos) const
{
	return 0;
}
float bmp::ALPHA16::RedF(bmp::buffIt_t pos,float v) const
{
	return 0;
}
float bmp::ALPHA16::GreenF(bmp::buffIt_t pos,float v) const
{
	return 0;
}
float bmp::ALPHA16::BlueF(bmp::buffIt_t pos,float v) const
{
	return 0;
}
float bmp::ALPHA16::AlphaF(bmp::buffIt_t pos,float v) const
{
	return SetFloat(pos,v);
}
float bmp::ALPHA16::BrightenessF(bmp::buffIt_t pos,float v) const
{
	return 0;
}
unsigned bmp::ALPHA16::NChannels() const
{
	return 1;
}
unsigned bmp::ALPHA16::BytesPerChannel() const
{
	return 2;
}
bool bmp::ALPHA16::IsAlpha() const
{
	return true;
}
