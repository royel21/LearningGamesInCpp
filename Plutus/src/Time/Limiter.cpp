#include "Limiter.h"

#include <thread>
#include <chrono>

#include "SleepImp.h"

#ifdef _WIN32
#include <windows.h>
#endif

namespace Plutus
{
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double>;

    Clock::time_point mStartPoint;
    std::chrono::duration<double> frameTime(0.0);
    std::chrono::duration<double> sleepAdjust(0.0);

    void Limiter::init(float fps)
    {
        mSpecFps = 1.0f / fps;
    }

    Limiter::~Limiter()
    {
    }

    float Limiter::start()
    {
        mStartPoint = Clock::now();

#ifdef __EMSCRIPTEN__
        Duration currentFrame = mStartPoint - mEndPoint;
        mEndPoint = mStartPoint;
        mLastElapsed = currentFrame.count();
#endif
        return (float)mLastElapsed;
    }

    void Limiter::end()
    {
#ifndef __EMSCRIPTEN__
        std::chrono::duration<double> target(mSpecFps);

        if (limitFPS) {
            Clock::time_point mEndPoint = Clock::now();
            std::chrono::duration<double> timeUsed = mEndPoint - mStartPoint;
            std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;

            if (sleepTime > std::chrono::duration<double>(0))
            {
#ifdef _WIN32
                timeBeginPeriod(1);
#endif
                std::this_thread::sleep_for(sleepTime);
#ifdef _WIN32
                timeEndPeriod(1);
#endif
            }
        }


        Duration frameTime = Clock::now() - mStartPoint;
        mLastElapsed = frameTime.count();

        // Compute the sleep adjustment using a low pass filter
        sleepAdjust = 0.995 * sleepAdjust + 0.1 * (target - frameTime);
#endif

        mFrameTime += mLastElapsed;
        mnFrameTime++;

        if (mFrameTime > 1.0f)
        {
            mFps = 1.0f / static_cast<float>(mFrameTime / mnFrameTime);
            mFrameTime = 0.0f;
            mnFrameTime = 0;
        }
    }


} // namespace Plutus
