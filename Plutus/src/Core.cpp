#include "Core.h"
#include "./Graphics/GLheaders.h"
#include "./Assets/AssetManager.h"
#include <GLFW/glfw3.h>

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
#if defined(__EMSCRIPTEN__) || defined(_WIN32)
        glfwSetWindowUserPointer(mWindow.getGLFWwindow(), this);
        glfwSetFramebufferSizeCallback(mWindow.getGLFWwindow(), [](GLFWwindow* window, int width, int height) {
            auto core = (Core*)glfwGetWindowUserPointer(window);
            core->Resize(width, height);
            });
#endif
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

    void Core::SetViewPortSize(float width, float height) {
        glViewport(0, 0, width, height);
    }
} // namespace Plutus