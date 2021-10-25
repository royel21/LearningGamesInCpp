#pragma once

#include "Window.h"
#include "ScreenList.h"

#include <Input/Input.h>
#include <Graphics/Shader.h>
#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch2D.h>

#include <Time/Limiter.h>

namespace Plutus
{
    class IGameScreen;

    class Engine
    {
    protected:
        int mScreenWidth = 1280;
        int mScreenHeight = 768;
        const char* mWinName;
        //Screen
        std::unique_ptr<ScreenList> mScreenList = nullptr;
        IGameScreen* mCurrentScreen = nullptr;
        // return the window object
        Window mWindow;
        ///Handler all the input of the window mouse, keyboard or gamepad
        Input* mInput = nullptr;
        Limiter mLimiter;

    public:
        Engine(const char* winName = "Default", int screenWidth = 1280, int screenHeight = 768);
        virtual ~Engine();
        // Run and initialize the game logic
        void run();

        virtual void addScreen() = 0;
        //Called on a the start of the window creation use for initialization
        virtual void onInit() = 0;
        virtual void onExit() = 0;
        virtual void onReisze(int width, int height) {};

        const float getFPS() { return mLimiter.getFPS(); }
        void setFPS(uint8_t fps) { mLimiter.setFPS((float)fps); }

        inline void setViewPort(int w, int h) { glViewport(0, 0, w, h); }

        //Set backgound color
        void setBackgoundColor(float r, float g, float b, float a)
        {
            glClearColor(r, g, b, a);
        }

        void setLimitFPS(bool isLimited) { mLimiter.setFPSLimiter(isLimited); }

        inline int getHeight() { return mScreenHeight; }
        inline int getWidth() { return mScreenWidth; }
        inline vec2f getWindowSize()
        {
            return { mScreenWidth, mScreenHeight };
        }

        inline Window* getWindow() { return &mWindow; }

    private:
        bool init();
        bool update(float dt);
        void draw();
        void exitGame();
        void cleanup();
        void onEvent(int code, int type);
    };
} // namespace Plutus
