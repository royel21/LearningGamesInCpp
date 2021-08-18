
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#include "Graphics/GLHeaders.h"
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>

#include "Window.h"
#include "./Input/Input.h"

#include <cstdlib>
#include <cstdio>
#include <unordered_map>

#ifdef _WIN32
#pragma comment(lib, "winmm.lib")
#endif

namespace Plutus
{
    const char* MouseKey[] = { "MouseLeft", "MouseRight", "MouseMidle", "", "" };

    std::unordered_map<int, const char*> unkeys;

    void iniKeys()
    {
        unkeys[32] = "Space";
        unkeys[341] = unkeys[345] = "Ctrl";
        unkeys[340] = unkeys[344] = "Shift";
        unkeys[335] = unkeys[257] = "Enter";
        unkeys[256] = "Scape";
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
        unkeys[268] = "End";
        unkeys[269] = "Home";
    }

    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto name = glfwGetKeyName(key, scancode);
        name = name ? name : unkeys[key];
        Input::getInstance()->keyStateChange(name, action > 0);
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        auto name = MouseKey[button];
        Input::getInstance()->keyStateChange(name, action > 0);
    }

    void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::getInstance()->setMouseCoords(static_cast<float>(xpos), static_cast<float>(ypos));
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Input::getInstance()->setMouseWheel(static_cast<int>(yoffset));
    }

    Window::~Window()
    {
#ifdef _WIN32
        timeEndPeriod(1);
#endif
        fprintf(stderr, "Reset TimeEndPeriod\n");
        if (mWindow != nullptr)
        {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }
    }

    bool Window::init(const char* name, int width, int height, GLFWwindow* parent)
    {
        iniKeys();
        // set a error call back for glfw internal error
        glfwSetErrorCallback(error_callback);
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

        //Print the OpenGL Version
        std::printf("*** OpenGL Version %s ***\n", glGetString(GL_VERSION));
        // set up the inputs callback
        glfwSetKeyCallback(mWindow, keyCallback);
        glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
        glfwSetScrollCallback(mWindow, scrollCallback);
        glfwSetCursorPosCallback(mWindow, mousePosCallback);
        //Enable alpha blend
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef _WIN32
        // if we are on window set the time precision to 1ms for sleep function
        timeBeginPeriod(1);
#endif
        std::printf("*** Ready***\n");
        return true;
    }

    bool Window::isFinish()
    {
        return glfwWindowShouldClose(mWindow);
    }

    void Window::update()
    {
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    }
}