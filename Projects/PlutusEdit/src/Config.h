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

        Entity mEnt;
        EditorProject() = default;
        void Copy(const EditorProject& proj);

        operator bool() const { return currentScene.empty(); }

        std::string getDir(const std::string& part);

        bool CreateScene(const std::string& name);
        void RenameScene(const std::string& oldName, const std::string& newName);
        void removeScene(std::string id);

        void clearScene();
    };

    struct Config
    {
        //Mouse Coords in view port
        Vec2f mMouseCoords;
        // Is Viewport hovered
        bool isHover;

        bool drawGrid = true;
        // Window Width
        int winWidth = 1280;
        // Window height
        int winHeight = 768;

        int tileWidth = 32;
        int tileHeight = 32;

        Camera2D* mCamera = nullptr;

        EditorProject mProject;

        EditorProject mTempProject;
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