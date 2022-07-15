#include "Limiter.h"

#include <chrono>

#include "SleepImp.h"

namespace Plutus
{
    using Clock = std::chrono::system_clock;
    using Duration = std::chrono::duration<float>;

    std::chrono::system_clock::time_point mStartPoint;
    std::chrono::system_clock::time_point mEndPoint;

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
        if (limitFPS) {
            Duration drawTime = Clock::now() - mStartPoint;
            if (drawTime.count() < mSpecFps)
            {
                msSleep((uint32_t)floorf((mSpecFps - drawTime.count()) * 1000.0f));
            }
        }
        Duration currentFrame = Clock::now() - mStartPoint;
        mLastElapsed = currentFrame.count();
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
