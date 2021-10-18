#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <ECS/Scene.h>
#include <Physics/Vectors.h>


namespace Plutus
{

    struct Project {
        int vpWidth = 1280;
        int vpHeight = 768;

        Entity mEnt;
        Ref<Scene> mScene;
        std::string mOpenScene;

        std::unordered_map<std::string, std::string> mScenes = {};

        std::unordered_map<std::string, std::string>& getItems() { return mScenes; }
        Project();

        void Create(const std::string& name);
        void add(const std::string& path);
        void Load(const std::string& path);
        void Save();
        void remove(std::string id) { mScenes.erase(id); }
    };

    struct Config
    {
        //Mouse Coords in view port
        vec2f mMouseCoords;
        // Is Viewport hovered
        bool isHover;
        // Window Width
        int winWidth = 1280;
        // Window height
        int winHeight = 768;
        Project* mProject;
        //Current Open Project
        std::string OpenProject = "";
        //List Of Project
        std::unordered_map<std::string, Project> mProjects = {};
        bool isLoaded = false;

        ~Config();

        // Create a Blank Project
        void CreateProj(const char* filePath);
        void LoadProject(const std::string& name);
        void RenameProj(const std::string& oldname, const std::string newName);

        static Config& get();

    private:
        Config() { Init(); };
        void Init();
        void save();
        void load();
    };
} // namespace Plutus