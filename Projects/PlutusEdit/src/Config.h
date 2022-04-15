#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <ECS/Scene.h>
#include <Math/Vectors.h>

#include <Utils/Pointer.h>

namespace Plutus
{
    class Camera2D;
    class Render;
    struct Config;

    enum State {
        Editing,
        Running
    };

    struct FloatColor {
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;
    };

    struct Project {
        int vpWidth = 1280;
        int vpHeight = 768;
        int wondowWidth = 1280;
        int windowHeight = 1280;

        float zoomLevel = 1.0f;
        vec4f bgColor = { 1 };

        Entity mEnt;
        Ref<Scene> mScene;
        Ref<Scene> mTempScene;
        std::string mOpenScene;

        Config* mConfig;

        std::unordered_map<std::string, std::string> mScenes = {};

        std::unordered_map<std::string, std::string>& getItems() { return mScenes; }
        Project();

        void  init();

        void Create(const std::string& name);
        void add(const std::string& path);
        void Load(const std::string& path);
        void Save();
        void remove(std::string id) { mScenes.erase(id); }

        void setZoom(float zoom = 1);
        void setBGColor(const vec4f color = { 1 });
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