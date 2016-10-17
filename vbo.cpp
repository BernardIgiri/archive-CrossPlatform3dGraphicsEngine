#include "stdafx.h"
#include "vbo.h"

using namespace vbo;


void VBOExt::VBOExtPtrs::Init()
{
    Log log;
    if (pGenBuffersARB==0);
        if (glfwExtensionSupported("GL_ARB_vertex_buffer_object"))
        {
            pGenBuffersARB=(PFNGLGENBUFFERSARBPROC)glfwGetProcAddress("glGenBuffersARB");
            pBindBufferARB=(PFNGLBINDBUFFERARBPROC)glfwGetProcAddress("glBindBufferARB");
            pBufferDataARB=(PFNGLBUFFERDATAARBPROC)glfwGetProcAddress("glBufferDataARB");
            pDeleteBuffersARB=(PFNGLDELETEBUFFERSARBPROC)glfwGetProcAddress("glDeleteBuffersARB");
            pIsBufferARB=(PFNGLISBUFFERARBPROC)glfwGetProcAddress("glIsBufferARB");
            pGetBufferParameterivARB=(PFNGLGETBUFFERPARAMETERIVARBPROC)glfwGetProcAddress("glGetBufferParameterivARB");
        }
        else
            throw ex::feature_not_found(log<<"Cannot used VBO's, GL_ARB_vertex_buffer_object not supported."<<lg::endl());
}

VBOExt::VBOExtPtrs VBOExt::extPtrs;
