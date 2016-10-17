#pragma once
#include "primatives.h"

namespace vbo
{
    class Log : public lg::Log
    {
        public:
        Log() : lg::Log("log.txt","VBO Object") {};
    };

    class VBOExt
    {
    private:
        static struct VBOExtPtrs
        {
            VBOExtPtrs() : pGenBuffersARB(0), pBindBufferARB(0), pBufferDataARB(0), pDeleteBuffersARB(0),pIsBufferARB(0),pGetBufferParameterivARB(0) {};

            void Init(); // initializes pointers

            PFNGLGENBUFFERSARBPROC pGenBuffersARB;					// VBO Name Generation Procedure
            PFNGLBINDBUFFERARBPROC pBindBufferARB;					// VBO Bind Procedure
            PFNGLBUFFERDATAARBPROC pBufferDataARB;					// VBO Data Loading Procedure
            PFNGLDELETEBUFFERSARBPROC pDeleteBuffersARB;			// VBO Deletion Procedure
            PFNGLISBUFFERARBPROC pIsBufferARB;
            PFNGLGETBUFFERPARAMETERIVARBPROC pGetBufferParameterivARB;
        } extPtrs;
    public:
        VBOExt() { extPtrs.Init(); };
        const PFNGLGENBUFFERSARBPROC GenBuffersARB() const { return extPtrs.pGenBuffersARB; };
        const PFNGLBINDBUFFERARBPROC BindBufferARB() const { return extPtrs.pBindBufferARB; };
        const PFNGLBUFFERDATAARBPROC BufferDataARB() const { return extPtrs.pBufferDataARB; };
        const PFNGLDELETEBUFFERSARBPROC DeleteBuffersARB() const { return extPtrs.pDeleteBuffersARB; };
        const PFNGLISBUFFERARBPROC IsBufferARB() const { return extPtrs.pIsBufferARB; };
        const PFNGLGETBUFFERPARAMETERIVARBPROC GetBufferParameterivARB() const { return extPtrs.pGetBufferParameterivARB; };
    };

    class VBO : boost::noncopyable
    {
    public:
        VBO() : id(GL_INVALID_VALUE) { vboExt.GenBuffersARB()(1,&id); };
        ~VBO() { vboExt.DeleteBuffersARB()(1,&id); };
        inline void Bind() const { vboExt.BindBufferARB()(GL_ARRAY_BUFFER_ARB,id); };
        VBOExt vboExt;
    private:
        GLuint id; // vbo id
    };

    enum VBOUsage_t
    {
        STATIC_DRAW = GL_STATIC_DRAW_ARB,
        STATIC_READ = GL_STATIC_READ_ARB,
        STATIC_COPY = GL_STATIC_COPY_ARB,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW_ARB,
        DYNAMIC_READ = GL_DYNAMIC_READ_ARB,
        DYNAMIC_COPY = GL_DYNAMIC_COPY_ARB,
        STREAM_DRAW = GL_STREAM_DRAW_ARB,
        STREAM_READ = GL_STREAM_READ_ARB,
        STREAM_COPY = GL_STREAM_COPY_ARB
    };

    namespace render
    {
        struct Vertex2D
        {
            Vertex2D() { glVertexPointer(2,GL_FLOAT,0,0); };
            typedef math::vector2f element_t;
        };
        struct Vertex3D
        {
            Vertex3D() { glVertexPointer(3,GL_FLOAT,0,0); };
            typedef math::vector3f element_t;
        };
        struct TexCoord
        {
            TexCoord() { glTexCoordPointer(2,GL_FLOAT,0,0); };
            typedef math::vector2f element_t;
        };
        struct Normal
        {
            Normal() { glNormalPointer(GL_FLOAT, 0, 0); };
            typedef math::vector3f element_t;
        };
        struct Color
        {
            Color() { glColorPointer(3, GL_FLOAT, 0, 0); };
            typedef math::vector4f element_t;
        };
    };

    template <typename ElementT>
    class RenderVBO
    {
    public:
        typedef typename ElementT::element_t element_t;
        RenderVBO() {};

        void Upload(const element_t elementArray[],unsigned nElements,VBOUsage_t usage)
        {
            vbo.Bind();
            vbo.vboExt.BufferDataARB()(GL_ARRAY_BUFFER_ARB,nElements*sizeof(element_t),elementArray,usage);
        }
        void Upload(const std::vector<element_t> &elementVector,VBOUsage_t usage)
        {
            vbo.Bind();
            vbo.vboExt.BufferDataARB()(GL_ARRAY_BUFFER_ARB,elementVector.size()*sizeof(element_t),&elementVector.front(),usage);
        }
        inline void Bind() const
        {
            vbo.Bind();
            ElementT();
        };
        inline void Render(unsigned nElements,prim::DrawMode_t drawMode) const
        {
            Bind();
            glDrawArrays(drawMode, 0, nElements);
        };
    private:
        VBO vbo; // vbo
    };


	template <typename FirstT,typename SecondT>
	class VBOPair
	{
    public:
	    VBOPair(unsigned nElements_,prim::DrawMode_t drawMode_) : nElements(nElements_),drawMode(drawMode_) {};

	    inline void SetUp(unsigned nElements_,prim::DrawMode_t drawMode_) { nElements=nElements_;drawMode=drawMode_; };

	    inline FirstT &First() { return first; };
	    inline const FirstT &First() const { return first; };
	    inline SecondT &Second() { return second; };
	    inline const SecondT &Second() const { return second; };

	    inline void Bind() const { first.Bind();second.Bind(); };

	    inline void Render() const { Render(nElements,drawMode); };
        inline void Render(unsigned nElements_,prim::DrawMode_t drawMode_) const { Bind();glDrawArrays(drawMode_, 0, nElements_); } ;

    private:
	    FirstT first;
	    SecondT second;
	    unsigned nElements; // number of elements
	    prim::DrawMode_t drawMode; // draw mode
	};

	template <typename FirstT,typename SecondT,typename ThirdT>
	class VBOTriple
	{
    public:
	    VBOTriple(unsigned nElements_,prim::DrawMode_t drawMode_) : nElements(nElements_),drawMode(drawMode_) {};

	    inline void SetUp(unsigned nElements_,prim::DrawMode_t drawMode_) { nElements=nElements_;drawMode=drawMode_; };

	    inline FirstT &First() { return first; };
	    inline const FirstT &First() const { return first; };
	    inline SecondT &Second() { return second; };
	    inline const SecondT &Second() const { return second; };
	    inline ThirdT &Third() { return third; };
	    inline const ThirdT &Third() const { return third; };

	    inline void Bind() const { first.Bind();second.Bind();third.Bind(); };

	    inline void Render() const { Bind();glDrawArrays(drawMode, 0, nElements); };
        inline void Render(unsigned nElements,prim::DrawMode_t drawMode) const { SetUp(nElements,drawMode);Render(); } ;
    private:
	    FirstT first;
	    SecondT second;
	    ThirdT third;
	    unsigned nElements; // number of elements
	    prim::DrawMode_t drawMode; // draw mode
	};
};
