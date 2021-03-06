#pragma once

#include <functional>

struct GLFWwindow;

namespace Plutus
{
    class Window
    {
    public:
        std::function<void(const char*)> onFileDrop = nullptr;
        std::function<void(int, int)> onResize = nullptr;

    public:
        Window() = default;
        Window(const char* name, int width, int height, GLFWwindow* parent = nullptr);
        ~Window();
        // initialize a window and gl 3.3 context
        bool init(const char* name, int width, int height, GLFWwindow* parent = nullptr);
        // tell is window should close
        bool isFinish();
        // swap backgound buffer with current buffer and pollevent
        void update();
        // tell glfw we want to close the window
        void close();

        void setAlwaysOnTOp(bool isOnTop);

        inline GLFWwindow* getGLFWwindow() { return mWindow; }

    private:
        GLFWwindow* mWindow;
        bool mAutoResizeVP = false;
    };
} // namespace Plutus