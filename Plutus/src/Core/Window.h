#pragma once

struct GLFWwindow;

namespace Plutus
{
    class Window
    {
    public:
        Window() = default;
        ~Window();
        // initialize a window and gl 3.3 context
        bool init(const char* name, int width, int height, GLFWwindow* parent = nullptr);

        void setBackgoundColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
        // tell is window should close
        bool isFinish();
        // swap backgound buffer with current buffer and pollevent
        void update();
        // tell glfw we want to close the window
        void close();

        inline GLFWwindow* getGLFWwindow() { return mWindow; }

    private:
        GLFWwindow* mWindow;
    };
} // namespace Plutus