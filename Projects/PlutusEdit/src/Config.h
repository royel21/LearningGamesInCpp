#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <ECS/Scene.h>
#include <Math/Vectors.h>
#include <Utils/Pointer.h>
#include <Core/Project.h>

namespace Plutus
{
    class Render;
    class Camera2D;

    enum State {
        Editing,
        Running
    };

    struct EditorProject : public Project {
        // Window Size
        std::string workingDir = "./";

        int tileWidth = 64;
        int tileHeight = 64;

        Entity mEnt;
        Ref<Scene> mTempScene;

        EditorProject();

        void CreateScene(const std::string& name);
        void removeScene(std::string id);
    };

    struct Config
    {
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

        EditorProject mProject;
        //Current Open Project
        std::string currentProject = "";
        std::string workingDir;
        //List Of Project
        std::unordered_map<std::string, std::string> mProjects;
        bool isLoaded = false;

        State state = Editing;
        Render* mRender;

        Config() { load(); }
        void init(Render* render);

        ~Config();

        // Create a Blank Project
        void CreateProj();
        void LoadProject(const std::string& name);

    private:
        void save();
        void load();
    };
} // namespace Plutus