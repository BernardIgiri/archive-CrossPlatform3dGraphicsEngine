// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <cml/cml.h>
#include <GL/glfw.h>

#ifndef GL_GLEXT_PROTOTYPES
    #define GL_GLEXT_PROTOTYPES 1
#endif

#include <GL/glext.h>
#include <zlib.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <stdio.h>
#include <iostream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <exception>
#include <boost/thread.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/functional/hash.hpp>
#include <png.h>
#ifdef WIN32
	#include <hash_map>
	#define IS_WINDOWS__ true
#else
	#include <tr1/unordered_map>
	#define IS_LINUX__ true
#endif

#include "log.h"
#include "iolib.h"
#include "myMath.h"
#include "algs.h"
#include "exceptions.h"

typedef boost::uint8_t byte;

// TODO: reference additional headers your program requires here
