#pragma once

namespace prim // primatives
{
	typedef std::vector<math::vector2f> TexCoords_t; // array of texture coordinates
	typedef math::vector4f color_t; // color value
	typedef std::vector<color_t> colors_t; // array of color values
	enum DrawMode_t {
		LINES=GL_LINES,
		LINE_LOOP=GL_LINE_LOOP,
		LINE_STRIP=GL_LINE_STRIP,
		POINTS=GL_POINTS,
		QUADS=GL_QUADS,
		QUAD_STRIP=GL_QUAD_STRIP,
		TRIANGLES=GL_TRIANGLES,
		POLYGONS=GL_POLYGON,
		TRIANGLE_FAN=GL_TRIANGLE_FAN,
		TRIANGLE_STRIP=GL_TRIANGLE_STRIP
	};
}
