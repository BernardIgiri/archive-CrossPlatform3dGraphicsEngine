#pragma once

namespace timing
{
    // calculates the frame rate based on the number of elapsed seconds
    inline double CalcFPS(boost::uint32_t nFrames,double elapsedSeconds) { return static_cast<double>(nFrames)/elapsedSeconds; };
    // object for acquiring elapsed time
    class Timer
    {
        public:

        Timer() : stamp(glfwGetTime()) {};
        inline double ElapsedSeconds() { double s=stamp;stamp=glfwGetTime();return stamp-s; };
        inline double PeakElapsedSeconds() { return stamp-glfwGetTime(); };

        private:

        double stamp; // last time stamp
    };
};
