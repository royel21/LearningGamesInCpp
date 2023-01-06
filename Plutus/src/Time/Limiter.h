#pragma once

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

    public:
        Limiter() { init(60); };
        ~Limiter();
        float start();
        void end();
        float getFPS() { return mFps; };
        void setFPS(float fps) { mSpecFps = 1.0f / fps; };
        void setFPSLimiter(bool state) { limitFPS = state; }
    private:
        void init(float fps);
    };
} // namespace Plutus