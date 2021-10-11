#pragma once
#include <chrono>

using TimePoint = std::chrono::system_clock::time_point;

namespace Plutus
{
    class Limiter
    {
        float mFps = 0;
        float mSpecFps = 0;
        double mFrameTime = 0;
        int mnFrameTime = 0;
        bool limitFPS = true;
        double mLastElapsed = 0;
        TimePoint mStartPoint;

    public:
        Limiter() : mSpecFps(1.0f / 60.0f) {};
        Limiter(float fps) : mSpecFps(1.0f / fps) {};
        float start();
        void end();
        float getFPS() { return mFps; };
        void setFPS(float fps) { mSpecFps = 1.0f / fps; };
        void setFPSLimiter(bool state) { limitFPS = state; }
    };
} // namespace Plutus