#pragma once

namespace iol
{
	// reads 1 T from the stream
	template <typename Stream,typename T>
	inline void Read(Stream &s,T &d) { s.read((char*)(&d),sizeof(T)); };

	// reads count objects into output iterator dest
	template <typename Stream,typename OutputIterator>
	void Read(Stream &s,OutputIterator &dest, boost::uint32_t count)
	{
		typename OutputIterator::value_type temp;
		while (count--)
		{
			Read(s,temp);
			*dest++=temp;
		}
	}
	// reads count objects into output iterator dest and uses temp as a read buffer
	template <typename Stream,typename OutputIterator,typename ValueT>
	void Read(Stream &s,OutputIterator &dest, boost::uint32_t count,ValueT temp)
	{
		while (count--)
		{
			Read(s,temp);
			*dest++=temp;
		}
	}

	// writes 1 T to the stream
	template <typename Stream,typename T>
	inline void Write(Stream &s,const T &d) { s.write((const char*)(&d),sizeof(T)); }

	// writes objects from begin to end to the stream
	template <typename Stream,typename InputIterator>
	inline void Write(Stream &s,InputIterator &begin, InputIterator &end)
	{ while (begin!=end) Write(s,*begin++);	}
};
