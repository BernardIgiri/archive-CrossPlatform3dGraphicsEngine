#pragma once
#include "stdafx.h"

namespace pool
{
	// loads objects not found in pool from SourceT source
	template <typename T,typename SourceT,typename ParamT=const char *>
	class Pool
	{
	public:
#ifdef WIN32
		typedef typename stdext::hash_map<ParamT,T> map_t;
#else
		typedef typename std::tr1::unordered_map<ParamT,T> map_t;
#endif
		Pool(SourceT source_) : source(source_) { };
		// retrieves reference T object specified by the param parameters
		void operator()(T &object,ParamT param)
		{
			typename map_t::iterator it=loadedMap.find(param);
			if (it==loadedMap.end())
			{ // not found then load and add it
				source(object,param);
				loadedMap[param]=object;
			}
			else // return it
				object=*it;
		};
	private:
		SourceT source; // input source object
		map_t loadedMap; // map of loaded objects
	};


	// Variation of pool that depends on smart pointers. Once an object loses its
	// last reference outside of SmartPool it is automatically deleted from the pool
	template <typename T,typename SourceT,typename ParamT=const char *>
	class SmartPool
	{
	public:
		typedef typename boost::shared_ptr<T> tPtr_t;
		typedef typename boost::weak_ptr<T> tWeakPtr_t;
#ifdef WIN32
		typedef typename stdext::hash_map<ParamT,tWeakPtr_t> map_t;
#else
		typedef typename std::tr1::unordered_map<ParamT,tWeakPtr_t> map_t;
#endif

		SmartPool(SourceT source_) : source(source_) { };
		SmartPool(const SmartPool &s) : source(s.source),loadedMap(s.loadedMap) { };
		SmartPool &operator=(const SmartPool &s) { source=s.source;loadedMap=s.loadedMap;return *this; };
		// retrieves reference T object specified by the param parameters
		void operator()(tPtr_t &ptr,ParamT param)
		{
			tWeakPtr_t wkPtr = loadedMap[param];
			if (wkPtr.expired())
			{ // not found then load and add it
				ptr.reset(new T());
				source(*ptr,param);
				wkPtr=ptr;
			}
			else // return it
				ptr=tPtr_t(wkPtr);
		};
	private:
		SourceT source; // input source object
		map_t loadedMap; // map of loaded objects
	};

	// wrapper for allowing Loaders to function as sources
	template <typename T,typename LoaderT,typename StreamT,
		typename ParamT=const char *>
	class Source
	{
	public:
		Source(LoaderT loader_,StreamT &stream_) : loader(loader_),stream(stream_) {};
		Source(const Source &s) : loader(s.loader),stream(s.stream) {};
		Source &operator=(const Source &s) { loader=s.loader;stream=s.stream;return *this; };
		void operator()(T &object,ParamT param)
		{
			stream.open(param);
			loader(object,stream);
		}
	private:
		LoaderT loader; // loader object
		StreamT &stream; // reference to stream object
	};

	// version of Source that sets the std::ios::open_mode to OPEN_MODE
	template <typename T,typename LoaderT,typename StreamT,
		typename ParamT=const char *>
	class SourceM
	{
	public:
        typedef int open_mode_t;
		SourceM(LoaderT loader_,StreamT &stream_,open_mode_t mode_) : loader(loader_),stream(stream_),mode(mode_) {};
		SourceM(const SourceM &s) : loader(s.loader),stream(s.stream),mode(s.mode) {};
		SourceM &operator=(const SourceM &s) { loader=s.loader;stream=s.stream;mode=s.mode;return *this; };
		void operator()(T &object,ParamT param)
		{
			stream.open(param,mode);
			loader(object,stream);
			stream.close();
		}
	private:
        open_mode_t mode; // open mode
		LoaderT loader; // loader object
		StreamT &stream; // reference to stream object
	};
}
