#include "Core.h"

#include <Input/Input.h>
#include <Graphics/GLheaders.h>

#ifdef __EMSCRIPTEN__
#include <functional>
#include <emscripten.h>
#endif

namespace Plutus
{
#ifdef __EMSCRIPTEN__
    std::function<void()> loop2;
    void main_loop2() { loop2(); }
#endif
    void Core::init() {
        mWindow.init(mName.c_str(), mWidth, mHeight);
        mCamera.init(mWidth, mHeight);

        mWindow.onResize = [&](int w, int h) {
            mWidth = w;
            mHeight = h;
            Resize(w, h);
        };

        mWindow.onFileDrop = [&](const char* file) {
            Dropfile(file);
        };

        Init();
    }

    void Core::Run() {
        init();
#ifdef __EMSCRIPTEN__
        loop2 = [&]
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
        emscripten_set_main_loop(main_loop2, 0, true);
#endif
        Exit();
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