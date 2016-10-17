#pragma once
#include "texture.h"
#include "primatives.h"
#include "vbo.h"

namespace prim // primatives
{
	namespace p2D
	{
		typedef std::vector<math::vector2i> Vertices_t;
		// 2D bounding box
		class BBOX
		{
		public:
			// default ctor
			BBOX() : upperLeft(0,0),lowerRight(0,0) {}
			// constructs bbox from verticies begin through end
			template <typename FwdItorT>
			BBOX(FwdItorT vertsBegin,FwdItorT vertsEnd)
			{
				int maxX=(*vertsBegin)[math::X],
					maxY=(*vertsBegin)[math::Y],
					minX=(*vertsBegin)[math::X],
					minY=(*vertsBegin)[math::Y];
				for (vertsBegin;vertsBegin!=vertsEnd;vertsBegin++)
				{
					minX=std::min(minX,(*vertsBegin)[math::X]);
					maxX=std::max(maxX,(*vertsBegin)[math::X]);
					minY=std::min(minY,(*vertsBegin)[math::Y]);
					maxY=std::max(maxY,(*vertsBegin)[math::Y]);
				}
				*this = BBOX(maxX,maxY,minX,minY);
			}
			// copy ctor
			BBOX(const BBOX& b) : upperLeft(b.upperLeft),lowerRight(b.lowerRight) {};
			// constructs bbox from upperLeft cordinate, and a width and height value
			BBOX(math::vector2i upperLeft_,unsigned width,unsigned height)
				: upperLeft(upperLeft_),lowerRight(upperLeft_[math::X]+width,upperLeft_[math::Y]-height) {};
			// constructs bbox from upperLeft and lowerRight coordinates
			BBOX(math::vector2i upperLeft_,math::vector2i lowerRight_)
				: upperLeft(upperLeft_),lowerRight(lowerRight_) {};
			// constructs bbox from min and max X and Y values
			BBOX(int maxX,int maxY,int minX,int minY) : upperLeft(maxX,minY),lowerRight(minX,maxY) {};
			// assingment operator
			inline BBOX &operator=(const BBOX& b) { upperLeft=b.upperLeft;lowerRight=b.lowerRight;return *this; };
			// returns max X value
			inline int MaxX() const { return lowerRight[math::X]; };
			// returns min X value
			inline int MinX() const { return upperLeft[math::X]; };
			// returns max Y value
			inline int MaxY() const { return upperLeft[math::Y]; };
			// returns min Y value
			inline int MinY() const { return lowerRight[math::Y]; };
			// returns width
			inline int Width() const { return MaxX()-MinX(); };
			// returns height
			inline int Height() const { return MaxY()-MinY(); };
			// returns upper left coordinate
			const math::vector2i &UpperLeft() const { return upperLeft; };
			// return lower right coordinate
			const math::vector2i &LowerRight() const { return lowerRight; };
			// returns true if BBOX's intersect
			inline bool Intersect(const BBOX& b)
			{
				return !((b.MaxX()<MinX())||(b.MaxY()<MinY())||
					(b.MinX()>MaxX())||(b.MinY()>MaxY()));
			}
			// translates BBOX coordinates by offest value
			inline void Translate(const math::vector2i &offset) { upperLeft+=offset;lowerRight+=offset; };
			// returns true if point is within BBOX
			inline bool Intersect(math::vector2i point)
			{
				return math::InRange(point[math::X],MinX(),MaxX())&&math::InRange(point[math::Y],MinY(),MaxY());
			}
		private:
			math::vector2i upperLeft, // upper left coordinate
							lowerRight; // lower right coordinate
		};
		typedef BBOX Rect2D; // 2D rectangle type

		namespace renderable
		{
		    const unsigned nRectVerts=4; // number of verticies in a rectangle
		    typedef vbo::RenderVBO<vbo::render::Vertex2D> ColoredRectVBO_t;
		    typedef vbo::VBOPair<vbo::RenderVBO<vbo::render::Vertex2D>,vbo::RenderVBO<vbo::render::TexCoord> > TexturedRectVBO_t;
		    // uploads the verticies for Rect2D to VBO
		    void RectVBO(ColoredRectVBO_t &vtxVBO,const Rect2D &rect);
		    // uploads the verticies for Rect2D to VBO
		    void RectVBO(TexturedRectVBO_t &vtxVBO,const Rect2D &rect);

		    enum RenderMode { SOLID=prim::TRIANGLE_FAN, OUTLINE=prim::LINE_LOOP };

		    class ColoredRect
		    {
            public:
                typedef ColoredRectVBO_t vbo_t;
                ColoredRect() : color(1.0f,1.0f,1.0f,1.0f) {};
                ColoredRect(math::vector2i position) : color(1.0f,1.0f,1.0f,1.0f) { Position(position); };
                ColoredRect(const Rect2D &rect,const color_t &color_) : bb(rect),color(color_)
                {
                   RectVBO(vtxVBO,rect);
                }
                ColoredRect(const ColoredRect& c) : bb(c.bb),color(c.color)
                {
                   RectVBO(vtxVBO,bb);
                }
                ColoredRect &operator=(const ColoredRect& c) { bb=c.bb; color=c.color; RectVBO(vtxVBO,bb); return *this; };
                inline void Resize(const Rect2D &rect) {
                    bb=rect;
                    RectVBO(vtxVBO,rect); };
                void Position(math::vector2i &position) { bb.Translate(bb.UpperLeft()-position); };
                const math::vector2i &Position() const { return bb.UpperLeft(); };
                void Render(const RenderMode mode=SOLID) const;
                const BBOX &OuterBounds() const { return bb; };
                const color_t &Color() const { return color; };
                color_t &Color() { return color; };
            private:
                color_t color; // color of rectangle
                vbo_t vtxVBO; // rectangle vb
                BBOX bb; // bounding box
		    };

		    class TexturedRect
		    {
            public:
                typedef TexturedRectVBO_t vbo_t;

                TexturedRect() : vtxVBO(nRectVerts,prim::TRIANGLE_FAN), color(1.0f,1.0f,1.0f,1.0f) {};
                TexturedRect(math::vector2i position) : vtxVBO(nRectVerts,prim::TRIANGLE_FAN), color(1.0f,1.0f,1.0f,1.0f) { Position(position); };
                TexturedRect(const Rect2D &rect,const color_t &color_)
                    : vtxVBO(nRectVerts,prim::TRIANGLE_FAN),bb(rect),color(color_)
                {
                   RectVBO(vtxVBO,rect);
                }
                TexturedRect(const TexturedRect& c) : bb(c.bb),color(c.color),vtxVBO(nRectVerts,prim::TRIANGLE_FAN)
                {
                   RectVBO(vtxVBO,bb);
                }
                TexturedRect &operator=(const TexturedRect& c) { bb=c.bb; color=c.color; RectVBO(vtxVBO,bb); return *this; };
                inline void Resize(const Rect2D &rect) {
                    bb=rect;
                    RectVBO(vtxVBO,rect); };
                void Position(math::vector2i &position) { bb.Translate(position-bb.UpperLeft()); };
                const math::vector2i &Position() const { return bb.UpperLeft(); };
                void Render(const RenderMode mode=SOLID) const;
                const BBOX &OuterBounds() const { return bb; };
                const color_t &Color() const { return color; };
                color_t &Color() { return color; };
                tex::Texture &Texture() { return texture; };
                const tex::Texture &Texture() const { return texture; };
            private:
                color_t color; // color of rectangle
                tex::Texture texture; // texture on rectangle
                vbo_t vtxVBO; // rectangle vb
                BBOX bb; // bounding box
		    };
		};
	};
};
