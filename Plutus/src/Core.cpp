#include "Core.h"

#include <Input/Input.h>
#include <Assets/temp/AssetManager.h>
#include <Graphics/GLheaders.h>

#ifdef __EMSCRIPTEN__
#include <functional>
#include <emscripten.h>
#endif

namespace Plutus
{
#ifdef __EMSCRIPTEN__
    std::function<void()> loop;
    void main_loop() { loop(); }
#endif

    Core::Core(const char* name, int width, int height) : mName(name), mWidth(width), mHeight(height) {
    }

    Core::~Core()
    {

    };

    void Core::init() {
        mWindow.init(mName.c_str(), mWidth, mHeight);
        mCamera.init(mWidth, mHeight);
        Setup();
    }

    void Core::Run() {
        init();
#ifdef __EMSCRIPTEN__
        loop = [&]
        {
#else
        while (mWindow.isFinish() && !mExit)
        {
#endif
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            float dt = mLimiter.start();
            mCamera.update();
            Update(dt);
            Draw();
            Input::get()->update();
            mWindow.update();
            mLimiter.end();

            if (mScreenList.size()) swapScreen();
        };
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
#endif
        Exit();

        AssetManager2::get()->destroy();
        }

    void Core::swapScreen() {
        if (!mNextScreen.empty()) {
            if (mScreenList.find(mNextScreen) != mScreenList.end()) {
                mCurrentScreen->Exit();
                mCurrentScreen = mScreenList[mNextScreen].get();
                mCurrentScreen->Enter();
                mNextScreen = "";
            }
        }
    }

    void Core::setNextScreen(const std::string & screenId) {
        mNextScreen = screenId;
    }
    } // namespace Plutus