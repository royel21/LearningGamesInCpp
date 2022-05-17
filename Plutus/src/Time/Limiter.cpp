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
#ifdef _WIN32
        // if we are on window set the time precision to 1ms for sleep function
        timeBeginPeriod(1);
#endif
        mStartPoint = Clock::now();
        return (float)mLastElapsed;
    }

    void Limiter::end()
    {
#ifdef __EMSCRIPTEN__
        Duration currentFrame = Clock::now() - mStartPoint;
        mLastElapsed = currentFrame.count();
#else
        if (limitFPS) {
            Duration drawTime = Clock::now() - mStartPoint;
            if (drawTime.count() < mSpecFps)
            {
                Sleep(uint32_t((mSpecFps - drawTime.count()) * 1000.0f));
            }
        }
        Duration currentFrame = Clock::now() - mStartPoint;
        mLastElapsed = currentFrame.count();
#endif
        mFrameTime += mLastElapsed;
        mnFrameTime++;

        if (mFrameTime > 1.000)
        {
            mFps = 1.0f / static_cast<float>(mFrameTime / mnFrameTime);
            mFrameTime = 0.0f;
            mnFrameTime = 0;
        }
    }


} // namespace Plutus
