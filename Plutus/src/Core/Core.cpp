#include "Core.h"

#include "IScreen.h"

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
        mWindow.init(mName.c_str(), mProject.winWidth, mProject.winHeight);
        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);

        if (!mProject.currentScene.empty()) {
            mProject.loadScene(mProject.currentScene);
        }

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
            if (mCurrentScreen) {
                mCurrentScreen->Update(dt);
                mCurrentScreen->Draw();
            }
            else {
                Update(dt);
                Draw();
            }
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

    void Core::addScreen(const std::string id, IScreen * screen, bool primary) {
        mScreenList[id] = screen;
        screen->init(this, &mCamera);
        screen->Init();
        if (primary) {
            mCurrentScreen = screen;
            screen->Enter();
        }
    }

    void Core::swapScreen() {
        if (!mNextScreen.empty()) {
            if (mScreenList.find(mNextScreen) != mScreenList.end()) {
                mCurrentScreen->Exit();
                mCurrentScreen = mScreenList[mNextScreen];
                mCurrentScreen->Enter();
                mNextScreen = "";
            }
        }
    }

    void Core::setNextScreen(const std::string & screenId) {
        mNextScreen = screenId;
    }

    void Core::setBackgoundColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }
} // namespace Plutus