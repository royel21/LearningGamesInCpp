#include "Core.h"
#include "./Graphics/GLheaders.h"
#include "./Assets/AssetManager.h"

namespace Plutus
{
#ifdef __EMSCRIPTEN__
    std::function<void()> loop;
    void main_loop() { loop(); }
#endif

    Core::Core()
    {
        mInput = Input::getInstance();
        mWindow.init(mName.c_str(), mWidth, mHeight);
        mCamera.init(mWidth, mHeight);
    };

    Core::Core(const char* name, int width, int height) : mName(name), mWidth(width), mHeight(height) {
        mWindow.init(name, width, height);
    }

    Core::~Core()
    {

    };

    void Core::init() {
    }

    void Core::Run() {
        init();
        Setup();

#ifdef __EMSCRIPTEN__
        loop = [&]
        {
#else
        while (mWindow.isFinish())
        {
#endif
            float dt = mLimiter.start();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            mCamera.update();
            mInput->update();
            Update(dt);
            Draw();
            mWindow.update();
            mLimiter.end();
        }
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, true);
#endif
        Exit();

        AssetManager::get()->clearData();
    }
} // namespace Plutus