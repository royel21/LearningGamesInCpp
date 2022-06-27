#include "Limiter.h"
#include <Graphics/GLheaders.h>
#ifdef _WIN32
#include <windows.h>
#endif

using Clock = std::chrono::system_clock;
using Duration = std::chrono::duration<float>;

namespace Plutus
{
    void Limiter::init(float fps)
    {
        mSpecFps = 1.0f / fps;
#ifdef _WIN32
        // if we are on window set the time precision to 1ms for sleep function
        timeBeginPeriod(1);
#endif
    }

    Limiter::~Limiter()
    {
#ifdef _WIN32
        timeEndPeriod(1);
#endif
    }

    float Limiter::start()
    {
        mStartPoint = Clock::now();
        return (float)mLastElapsed;
    }

    void Limiter::end()
    {
#ifndef __EMSCRIPTEN__
        if (limitFPS) {
            Duration drawTime = Clock::now() - mStartPoint;
            if (drawTime.count() < mSpecFps)
            {
                auto slpTime = (uint32_t)floorf((mSpecFps - drawTime.count()) * 1000.0f);
                Sleep(slpTime);
            }
        }
#endif
        Duration currentFrame = Clock::now() - mStartPoint;
        mLastElapsed = currentFrame.count();

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
