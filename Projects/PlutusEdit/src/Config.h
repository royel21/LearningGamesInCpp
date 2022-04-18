#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <ECS/Scene.h>
#include <Math/Vectors.h>

#include <Utils/Pointer.h>

namespace Plutus
{
    class Render;
    class Camera2D;

    enum State {
        Editing,
        Running
    };

    struct Project {
        // Window Size
        int windowWidth = 1280;
        int windowHeight = 768;
        // View Port Size
        int vpWidth = 1280;
        int vpHeight = 768;

        float zoomLevel = 1.0f;
        //FPS Limiter
        float FPS = 60.0f;
        /*********************** Box2d Config ***********************/
        // Box2d Velocity Iteration
        int velIter = 8;
        // Box2d Position Iteration
        int positionIter = 3;
        // Box2d Time step FPS 
        float timeStepInSec = 60.0f;
        // Box2d gravity
        vec2f gravity = { 0.0f, -9.8f };
        // Box2d Auto Clear Force
        bool autoClearForce = true;

        Entity mEnt;
        Ref<Scene> mScene;
        Ref<Scene> mTempScene;
        std::string mOpenScene;

        std::unordered_map<std::string, std::string> mScenes = {};

        Project();

        void init();

        void Create(const std::string& name);
        void add(const std::string& path);
        void Load(const std::string& path);
        void Save();
        void remove(std::string id) { mScenes.erase(id); }
    };

    struct Config
    {
        float vpZoom = 1.0f;
        vec4f vpColor = { 1.0f };
        vec2f vpPos;
        //Mouse Coords in view port
        vec2f mMouseCoords;
        // Is Viewport hovered
        bool isHover;

        bool drawGrid = true;
        // Window Width
        int winWidth = 1280;
        // Window height
        int winHeight = 768;

        Camera2D* mCamera;

        Project* mProject;
        //Current Open Project
        std::string OpenProject = "";
        //List Of Project
        std::unordered_map<std::string, Project> mProjects = {};
        bool isLoaded = false;

        State state = Editing;
        Render* mRender;

        Config() { load(); }
        void init(Render* render);

        ~Config();

        // Create a Blank Project
        void CreateProj(const char* filePath);
        void LoadProject(const std::string& name);
        void RenameProj(const std::string& oldname, const std::string newName);

    private:
        void save();
        void load();
    };
} // namespace Plutus