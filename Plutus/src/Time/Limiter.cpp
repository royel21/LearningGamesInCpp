#include "Limiter.h"
#include <windows.h>
#include <Graphics/GLheaders.h>


using Clock = std::chrono::system_clock;
using Duration = std::chrono::duration<float>;

namespace Plutus
{

    float Limiter::start()
    {
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
