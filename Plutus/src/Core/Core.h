#pragma once

#include <string>
#include <unordered_map>

#include <Core/Window.h>
#include <Time/Limiter.h>
#include <Graphics/Camera2D.h>

#include "Project.h"


namespace Plutus
{
    class IScreen;
    /**
     * Base Class for create an app
     * @param mWidth
     * @param mHeight
     * @param mCamera
     * @param Init virtual void Init called once when the app is starting
     * @param Update virtual void Uptade(float) called every frame
     * @param Draw virtual void Draw called every frame
     * @param Exit virtual void Exit called when the app is being destroyed use for cleanup
     * @param Resize virtual void Resize(int, int) called when the app is resizing
     * @param Dropfile virtual void Dropfile(const char*) called a file is dropped in the app
     */
    class Core
    {
    protected:
        Window mWindow;
        Limiter mLimiter;

        bool mUseConfig = false;

        //Width of the Window
        int mWidth = 1280;
        //Height of the Window
        int mHeight = 768;

        bool mExit = false;
        //Title of the Window
        std::string mName = "Plutus App";
        //Game Main Camera
        Camera2D mCamera;

        std::string mNextScreen;
        IScreen* mCurrentScreen = nullptr;
        std::unordered_map<std::string, IScreen*> mScreenList;

    public:
        Project mProject;

    public:
        Core() = default;
        //Run the game loop, only return when the app exit.
        void Run();
        //Called after the window creation. 
        virtual void Init() {};
        //Called on the loop update, should be overrided.
        virtual void Update(float dt) {};
        //Called on the loop draw, should be overrided.
        virtual void Draw() {};
        //Called before the window exit, should be overrided.
        virtual void Exit() { };
        //Called whenever window resize
        virtual void Resize(int width, int height) {}
        //Called whenever window resize
        virtual void Dropfile(const char* file) {}
        // FPS on micro seconds 0.016 millis by default. 
        inline float getFPS() { return mLimiter.getFPS(); }
        //Set the fps of the game loop.
        inline void setFPS(float fps) { mLimiter.setFPS(fps); }
        //set if fps should be limited.
        void limitFPS(bool state) { mLimiter.setFPSLimiter(state); }

        void addScreen(const std::string id, IScreen* screen, bool primary = false);
        void setNextScreen(const std::string& screenId);

        inline void setAlwaysOnTop(bool isTop = true) { mWindow.setAlwaysOnTOp(isTop); }

        void printGLVersion();

        void setBackgoundColor(float r = 1, float g = 1, float b = 1, float a = 1);

    private:
        void init();
        void swapScreen();
    };
} // namespace Plutus