#pragma once


namespace math
{
	using namespace cml;
	// clamps value to range min-max
	template <typename T>
	inline T Clamp(T value,const T min,const T max) { return std::min(max,std::max(min,value)); };
	// wraps value to values within the range of min-max
	inline boost::uint32_t Wrap(boost::uint32_t value,const boost::uint32_t min,const boost::uint32_t max) { return value%(max-min)+min; };
	// wraps value to values within the range of min-max
	inline float Wrap(float value,const float min,const float max)
	{
		float range=(max-min);
		float ratio=value/range;
		return (ratio-static_cast<int>(ratio))*range+min;
	}
	// wraps value to values within the range of min-max without considering overflows greater than max*2 and -min*2
	template <typename T>
	inline T WrapQ(T value,const T min,const T max) { if (value<min) return value+min;else if (value>max) return max-value;return value; };
	// returns true if value is within range min max
	template<typename T>
	inline bool InRange(T value,T min,T max) { return (value>=min)&&(value<=max); };
	// returns true if value is exclusively within range min max
	template<typename T>
	inline bool InRangeX(T value,T min,T max) { return (value>min)&&(value<max); };

	// useful enums for vectors
	// typical indices for given axises
	enum Axis { X=0,Y,Z };
	// indices for color values
	enum Color { R=0,G,B,A };
	// indices for color values
	enum ColorF { RED=0,GREEN,BLUE,ALPHA };
	// indices for texture cordinates
	enum UVCoordinates { U=0,V,W };

	typedef cml::vector<int,cml::fixed<2> > vector2i;
	typedef cml::vector<int,cml::fixed<3> > vector3i;
	typedef cml::vector<int,cml::fixed<4> > vector4i;
};
