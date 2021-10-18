#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <ECS/Scene.h>
#include <Physics/Vectors.h>


namespace Plutus
{
    struct ViewPortState {
        int width = 1280;
        int height = 768;
        vec2f mouseCoords;
        bool isHover;
    };

    struct Project {
        Entity mEnt;
        Ref<Scene> mScene;
        std::string mOpenScene;
        ViewPortState mVpState;

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
        int winWidth = 1280;
        int winHeight = 768;
        Project* mProject;
        std::string OpenProject = "";
        std::unordered_map<std::string, Project> mProjects = {};
        Config() {}
        ~Config();
        void Init();
        bool loadConfig();
        void CreateProj(const char* filePath);
        static Config& get();

    private:
        void save();
        void load();
    };
} // namespace Plutus