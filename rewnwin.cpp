#include "stdafx.h"
#include "renwin.h"
#include <string>

namespace renwin
{
    class Log : public lg::Log
    {
        public:
        Log() : lg::Log("log.txt","Render Window") {};
    };
}

using namespace renwin;

Window::Window(const std::string &appTitle,math::vector2i &resolution,bool isFullscreen)
{
    Log log;
    if( !glfwOpenWindow( resolution[math::X], resolution[math::Y], 0, 0, 0, 0, 16, 0, (isFullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW) ) )
       throw ex::open_failed_error(log<<"Failed to open window: "<<appTitle<<lg::endl());
    glfwSetWindowTitle(appTitle.c_str());
}

void Window::SetResolution(const math::vector2i &resolution)
{
    boost::mutex::scoped_lock lock(mutex);
    glfwSetWindowSize(resolution[math::X],resolution[math::Y]);
    ResChangeTrigger t(resolution);
    resChangeEvent.Trigger(t);
}
