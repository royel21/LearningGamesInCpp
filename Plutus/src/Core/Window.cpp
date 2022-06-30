
#include "Window.h"

#pragma warning(disable: 4005)

#include <Graphics/GLheaders.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#elif _WIN32
extern "C"
{
#include <windows.h>
}
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#pragma comment(lib, "winmm.lib")
#endif

extern "C" {
    // _declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    // _declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;
}

#include <Input/Input.h>

#include <array>

#include <Log/Logger.h>
#include <Time/Timer.h>

namespace Plutus
{
    std::array<const char*, 1024> unkeys;
    void initKeys();

    const char* getKey(int key) {
        return unkeys[key] ? unkeys[key] : "Unkown";
    }

    Window::Window(const char* name, int width, int height, GLFWwindow* parent)
    {
        init(name, width, height, parent);
    }

    bool Window::init(const char* name, int width, int height, GLFWwindow* parent)
    {
        initKeys();
        // set a error call back for glfw internal error
        glfwSetErrorCallback([](int error, const char* description) {

            });
        //Initialise glfw stuff
        if (!glfwInit())
        {
            exit(EXIT_FAILURE);
            return false;
        }
        // Select the opengl version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // Create the window
        mWindow = glfwCreateWindow(width, height, name, NULL, NULL);
        if (!mWindow)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
            return false;
        }

        //Create the gl context on this window
        glfwMakeContextCurrent(mWindow);
#ifndef __EMSCRIPTEN__
        //Load the OpenGL Context
        gladLoadGL();
#endif
        // Set a pointer to my window class
        glfwSetWindowUserPointer(mWindow, this);
        // Setup all callback
        glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Input::get()->keyStateChange(getKey(key), action);
            });
        // Mouse Button 
        glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
            Input::get()->keyStateChange(getKey(button), action > 0);
            });
        // Mouse Move
        glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos) {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            Input::get()->setMouseCoords(static_cast<float>(xpos), static_cast<float>(height - ypos));
            });
        // Mouse Wheel 
        glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
            Input::get()->setMouseWheel(static_cast<int>(yoffset));
            });
        //Item Dropped to the window
        glfwSetDropCallback(mWindow, [](GLFWwindow* window, int count, const char** paths) {
            auto win = (Window*)glfwGetWindowUserPointer(window);
            if (win->onFileDrop != nullptr) {
                for (int i = 0; i < count; i++)
                    win->onFileDrop(paths[i]);
            }
            });
        // Window Reisze Event
        glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
            auto win = (Window*)glfwGetWindowUserPointer(window);
            if (win->onResize) {
                win->onResize(width, height);
            }
            });

        return true;
    }
    Window::~Window()
    {
        if (mWindow != nullptr)
        {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }
    }

    bool Window::isFinish()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return !glfwWindowShouldClose(mWindow);
    }

    void Window::update()
    {
        glfwSwapBuffers(mWindow);
        Input::get()->update();
        glfwPollEvents();
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    }

    void Window::setAlwaysOnTOp(bool isOnTop)
    {
        if (mWindow)
            glfwSetWindowAttrib(mWindow, GLFW_FLOATING, isOnTop);
    }

    void Window::setVSYNC(int state)
    {
        glfwSwapInterval(state);
    }

    void Window::setTitle(const char* title)
    {
        glfwSetWindowTitle(mWindow, title);
    }

    void initKeys()
    {
        unkeys[0] = "MouseLeft";
        unkeys[1] = "MouseRight";
        unkeys[2] = "MouseMidle";
        unkeys[3] = "MouseEx1";
        unkeys[4] = "MouseEx2";
        unkeys[32] = "Space";
        unkeys[39] = "'";
        unkeys[44] = ",";
        unkeys[45] = "-";
        unkeys[46] = ".";
        unkeys[47] = "/";
        unkeys[48] = "0";
        unkeys[49] = "1";
        unkeys[50] = "2";
        unkeys[51] = "3";
        unkeys[52] = "4";
        unkeys[53] = "5";
        unkeys[54] = "6";
        unkeys[55] = "7";
        unkeys[56] = "8";
        unkeys[57] = "9";
        unkeys[61] = "+";
        unkeys[65] = "A";
        unkeys[66] = "B";
        unkeys[67] = "C";
        unkeys[68] = "D";
        unkeys[69] = "E";
        unkeys[70] = "F";
        unkeys[71] = "G";
        unkeys[72] = "H";
        unkeys[73] = "I";
        unkeys[74] = "J";
        unkeys[75] = "K";
        unkeys[76] = "L";
        unkeys[77] = "M";
        unkeys[78] = "N";
        unkeys[79] = "O";
        unkeys[80] = "P";
        unkeys[81] = "Q";
        unkeys[82] = "R";
        unkeys[83] = "S";
        unkeys[84] = "T";
        unkeys[85] = "U";
        unkeys[86] = "V";
        unkeys[87] = "W";
        unkeys[88] = "X";
        unkeys[89] = "Y";
        unkeys[90] = "Z";
        unkeys[91] = "[";
        unkeys[92] = "\\";
        unkeys[93] = "]";
        unkeys[94] = "`";
        unkeys[256] = "Scape";
        unkeys[257] = "Enter";
        unkeys[258] = "Tab";
        unkeys[260] = "Back";
        unkeys[260] = "Ins";
        unkeys[261] = "Del";
        unkeys[262] = "Right";
        unkeys[263] = "Left";
        unkeys[264] = "Down";
        unkeys[265] = "Up";
        unkeys[266] = "PageUp";
        unkeys[267] = "PageDown";
        unkeys[290] = "F1";
        unkeys[291] = "F2";
        unkeys[292] = "F3";
        unkeys[293] = "F4";
        unkeys[294] = "F5";
        unkeys[295] = "F6";
        unkeys[296] = "F7";
        unkeys[297] = "F8";
        unkeys[298] = "F9";
        unkeys[299] = "F10";
        unkeys[300] = "F11";
        unkeys[301] = "F12";
        unkeys[320] = "NUMPAD0";
        unkeys[321] = "NUMPAD1";
        unkeys[322] = "NUMPAD2";
        unkeys[323] = "NUMPAD3";
        unkeys[324] = "NUMPAD4";
        unkeys[325] = "NUMPAD5";
        unkeys[326] = "NUMPAD6";
        unkeys[327] = "NUMPAD7";
        unkeys[328] = "NUMPAD8";
        unkeys[329] = "NUMPAD9";
        unkeys[330] = ".";
        unkeys[331] = "/";
        unkeys[332] = "*";
        unkeys[333] = "-";
        unkeys[334] = "+";
        unkeys[335] = "Enter";
        unkeys[340] = "Shift";
        unkeys[341] = "Ctrl";
        unkeys[342] = "Alt";
        unkeys[344] = "Shift";
        unkeys[345] = "Ctrl";
        unkeys[346] = "AltRight";
    }
}