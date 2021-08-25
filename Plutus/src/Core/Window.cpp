
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
    std::unordered_map<int, const char *> unkeys;

    void initKeys();

    static void error_callback(int error, const char *description)
    {
        std::printf("Error: %s\n", description);
    }

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto name = unkeys[key];
        Input::getInstance()->keyStateChange(name == nullptr ? "Unkown" : name, action > 0);
    }

    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        Input::getInstance()->keyStateChange(unkeys[button], action > 0);
    }

    void mousePosCallback(GLFWwindow *window, double xpos, double ypos)
    {
        Input::getInstance()->setMouseCoords(static_cast<float>(xpos), static_cast<float>(ypos));
    }

    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Input::getInstance()->setMouseWheel(static_cast<int>(yoffset));
    }
    void drop_callback(GLFWwindow *window, int count, const char **paths)
    {
        for (int i = 0; i < count; i++)
            if (Input::getInstance()->onFileDrop != nullptr)
                Input::getInstance()->onFileDrop(paths[i]);
    }
    Window::~Window()
    {
#ifdef _WIN32
        timeEndPeriod(1);
#endif
        std::printf("Reset TimeEndPeriod\n");
        if (mWindow != nullptr)
        {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }
    }

    bool Window::init(const char *name, int width, int height, GLFWwindow *parent)
    {
        initKeys();
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
        glfwSetDropCallback(mWindow, drop_callback);
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

    void initKeys()
    {
        unkeys = {
            {0, "MouseLeft"},
            {1, "MouseRight"},
            {2, "MouseMidle"},
            {3, "MouseEx1"},
            {4, "MouseEx2"},
            {32, "Space"},
            {39, "'"},
            {44, ","},
            {45, "-"},
            {46, "."},
            {47, "/"},
            {48, "0"},
            {49, "1"},
            {50, "2"},
            {51, "3"},
            {52, "4"},
            {53, "5"},
            {54, "6"},
            {55, "7"},
            {56, "8"},
            {57, "9"},
            {65, "A"},
            {66, "B"},
            {67, "C"},
            {68, "D"},
            {69, "E"},
            {70, "F"},
            {71, "G"},
            {72, "H"},
            {73, "I"},
            {74, "J"},
            {75, "K"},
            {76, "L"},
            {77, "M"},
            {78, "N"},
            {79, "O"},
            {80, "P"},
            {81, "Q"},
            {82, "R"},
            {83, "S"},
            {84, "T"},
            {85, "U"},
            {86, "V"},
            {87, "W"},
            {88, "X"},
            {89, "Y"},
            {90, "Z"},
            {91, "["},
            {92, "\\"},
            {93, "]"},
            {94, "`"},
            {256, "Scape"},
            {258, "Tab"},
            {260, "Back"},
            {260, "Ins"},
            {261, "Del"},
            {262, "Right"},
            {263, "Left"},
            {264, "Down"},
            {265, "Up"},
            {266, "PageUp"},
            {267, "PageDown"},
            {290, "F1"},
            {291, "F2"},
            {292, "F3"},
            {293, "F4"},
            {294, "F5"},
            {295, "F6"},
            {296, "F7"},
            {297, "F8"},
            {298, "F9"},
            {299, "F10"},
            {300, "F11"},
            {301, "F12"},
            {320, "NUMPAD0"},
            {321, "NUMPAD1"},
            {322, "NUMPAD2"},
            {323, "NUMPAD3"},
            {324, "NUMPAD4"},
            {325, "NUMPAD5"},
            {326, "NUMPAD6"},
            {327, "NUMPAD7"},
            {328, "NUMPAD8"},
            {329, "NUMPAD9"},
            {331, "NUMPAD/"},
            {332, "NUMPAD*"},
            {333, "NUMPAD-"},
            {334, "NUMPAD+"},
            {335, "NUMPAD."},
            {340, "Shift"},
            {341, "Ctrl"},
            {342, "Alt"},
        };
    }
}