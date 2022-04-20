
#pragma once

#include <string>

#include <unordered_map>
#include <Math/Vectors.h>
#include <Utils/Pointer.h>

namespace Plutus
{
    class Scene;

    struct Project {
        std::string winTitle = "Plutus App";
        // Window Size
        int winWidth = 1280;
        int winHeight = 768;
        // View Port Size
        int vpWidth = 1280;
        int vpHeight = 768;
        vec2f vpPos = { 0, 0 };

        float zoomLevel = 1.0f;
        //FPS Limiter
        float maxFPS = 60.0f;
        /*********************** Box2d Config ***********************/
        // Box2d Velocity Iteration
        int velIter = 8;
        // Box2d Position Iteration
        int positionIter = 3;
        // Box2d Time step FPS 
        float timeStepInSec = 1.0f / maxFPS;
        // Box2d gravity
        vec2f gravity = { 0.0f, -9.8f };
        // Box2d Auto Clear Force
        bool autoClearForce = true;

        Ref<Scene> scene;
        std::string currentScene;
        std::string currentScenePath;

        Project();

        void clear();

        void load(const std::string& path);
        void save(const std::string& path);

        void loadScene(const std::string& path);
        void saveScene();
    };
} // namespace Plutus
