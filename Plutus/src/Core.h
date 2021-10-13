#pragma once
#include "./Core/Window.h"
#include "./Time/Limiter.h"
#include "./Input/Input.h"
#include "./Graphics/Camera2d.h"

#include <string>

namespace Plutus
{
    class Core
    {
    private:
        Window mWindow;
        Limiter mLimiter;
    protected:
        //Width of the Window
        int mWidth = 1280;
        //Height of the Window
        int mHeight = 768;
        //Title of the Window
        std::string mName = "Plutus App";
        //Game Main Camera
        Camera2D mCamera;
        //InputManager Reference
        Input* mInput;

    public:
        Core();
        Core(const char* name, int width, int height);
        ~Core();
        //Run the game loop, only return when the app exit.
        void Run();
        //Called after the window creation. 
        virtual void Setup() {};
        //Called on the loop update, should be overrided.
        virtual void Update(float dt) {};
        //Called on the loop draw, should be overrided.
        virtual void Draw() {};
        //Called before the window exit, should be overrided.
        virtual void Exit() { };
        virtual void Resize(int width, int height) {}
        // FPS on micro seconds 0.016 millis by default. 
        float getFPS() { return mLimiter.getFPS(); }
        //Set the fps of the game loop.
        void setFPS(float fps) { mLimiter.setFPS(fps); }
        //set if fps should be limited.
        void limitFPS(bool state) { mLimiter.setFPSLimiter(state); }

        void SetViewPortSize(float width, float height);
    private:
        void init();
    };
} // namespace Plutus