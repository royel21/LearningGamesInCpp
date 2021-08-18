#pragma once

#include <memory>
#include "Input/Input.h"
#include "Graphics/Shader.h"
#include "glm/glm.hpp"
#include "Window.h"
#include "ScreenList.h"

namespace Plutus
{
    class IGameScreen;

    class Engine
    {
    protected:
        float mFps = 0.0f;
        bool mIsRunning = false;
        int mScreenWidth = 1280;
        int mScreenHeight = 768;
        const char* mWinName;
        bool limitFPS = true;
        double mSpecFPS = 0.016666667;
        //Screen
        std::unique_ptr<ScreenList> mScreenList = nullptr;
        IGameScreen* mCurrentScreen = nullptr;
        // return the window object
        Window mWindow;
        ///Handler all the input of the window mouse, keyboard or gamepad
        Input* mInput = nullptr;

    public:
        Engine(const char* winName = "Default", int screenWidth = 1280, int screenHeight = 768);
        virtual ~Engine();
        // Run and initialize the game logic
        void run();

        virtual void addScreen() = 0;
        //Called on a the start of the window creation use for initialization
        virtual void onInit() = 0;
        virtual void onExit() = 0;

        const float getFPS() const { return mFps; }
        void setFPS(uint8_t fps) { mSpecFPS = 1.0f / fps; }

        inline void setViewPort(int w, int h) { glViewport(0, 0, w, h); }

        //Set backgound color
        void setBackgoundColor(float r, float g, float b, float a)
        {
            glClearColor(r, g, b, a);
        }

        void setLimitFPS(bool isLimited) { limitFPS = isLimited; }

        inline int getHeight() { return mScreenHeight; }
        inline int getWidth() { return mScreenWidth; }
        inline glm::vec2 getWindowSize()
        {
            return { mScreenWidth, mScreenHeight };
        }

        inline Window* getWindow() { return &mWindow; }

    private:
        bool init();
        bool update(float dt);
        void draw();
        void checkEvent();
        void exitGame();
        void cleanup();
        void onEvent(int code, int type);

    private:
        double mLastElapsed = 0.0f;
        double mFrameTime = 0;
        int mnFrameTime = 0;
    };
} // namespace Plutus
