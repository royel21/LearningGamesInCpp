
#pragma once

#include <string>

#include <unordered_map>
#include <Math/Vectors.h>
#include <Utils/Pointer.h>

namespace Plutus
{
    class Scene;

    struct Project {
        bool isLoaded = false;
        std::string winTitle = "Plutus App";
        // Window Size
        int winWidth = 1280;
        int winHeight = 768;
        // View Port Size
        int vpWidth = 1280;
        int vpHeight = 768;
        int tileWidth = 32;
        int tileHeight = 32;
        Vec2f vpPos = { 0, 0 };

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
        Vec2f gravity = { 0.0f, -9.8f };
        // Box2d Auto Clear Force
        bool autoClearForce = true;

        Ref<Scene> scene;
        std::string currentScene;
        std::string currentScenePath;
        std::unordered_map<std::string, std::string> scenes;

        Project();

        operator bool() const { return isLoaded; }

        void clear();

        template<typename T>
        Vec4f getRect(const T& tile) {
            return { float(tile.x * tileWidth), float(tile.y * tileHeight), tileWidth, tileHeight };
        }

        inline Vec4f getRect(float x, float y) {
            return { x * tileWidth, y * tileHeight, tileWidth, tileHeight };
        }

        inline Vec4f getRect(int x, int y) { return getRect(float(x), float(y)); }

        void load(const std::string& path);
        void save(const std::string& path);

        void loadScene(const std::string& path);
        void unLoadScene();
        void saveScene();
    };
} // namespace Plutus
