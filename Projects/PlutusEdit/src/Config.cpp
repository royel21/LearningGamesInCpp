#include "Config.h"

#include <math.h>
#include <fstream>  
#include <iostream>
#include <filesystem>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Assets/AssetManager.h>

#include <Serialize/SceneLoader.h>
#include <Serialize/SceneSerializer.h>

#include "Helpers/Render.h"
#include "Helpers/ConfigHelper.h"

namespace Plutus
{
    Config::~Config() {
        save();
    }

    void Config::load() {

        if (!std::filesystem::exists("assets"))
        {
            std::filesystem::create_directories("assets/textures");
            std::filesystem::create_directories("assets/sounds");
            std::filesystem::create_directories("assets/fonts");
            std::filesystem::create_directories("assets/scenes");
        }
        loadConfig(this);
    }

    void Config::save() { SaveConfig(this); }

    void Config::init(Render* render)
    {
        mRender = render;

        if (mProject->mScenes.size())
            mProject->Load(mProject->mScenes[mProject->mOpenScene]);

        mRender->init(this);
    }

    void Config::CreateProj(const char* name)
    {
        mProject = &mProjects[name];
        OpenProject = name;

        mRender->reload(this);
    }

    void Config::LoadProject(const std::string& name) {
        mProject = &mProjects[name];
        if (mProject->mScenes.size()) {
            auto found = mProject->mScenes.find(mProject->mOpenScene);
            if (found != mProject->mScenes.end()) {
                mProject->Load(found->second);
            }
        }
        else {
            mProject->mScene = CreateRef<Scene>();
            mProject->mTempScene = CreateRef<Scene>();
        }

        mRender->reload(this);
    }

    void Config::RenameProj(const std::string& oldname, const std::string newName) {
        mProjects[newName] = mProjects[oldname];
        mProjects.erase(oldname);
        if (OpenProject.compare(oldname) == 0) {
            mProject = &mProjects[newName];
            OpenProject = newName;
        }
    }

    Project::Project()
    {
        mScene = CreateRef<Scene>();
        mTempScene = CreateRef<Scene>();
    }

    void Project::init()
    {
        vpWidth = 1280;
        vpHeight = 768;
        windowWidth = 1280;
        windowHeight = 1280;

        zoomLevel = 1.0f;

        mEnt = {};
        mOpenScene = "";
        mScene = CreateRef<Scene>();
        mTempScene = CreateRef<Scene>();
    }

    void Project::Create(const std::string& name)
    {
        auto nName = name + ".json";
        auto found = mConfig->mProject->mScenes.find(name);

        if (found == mScenes.end()) {
            auto newScene = "assets/scenes/" + name;
            if (Utils::createFile(newScene.c_str(), "{}")) {
                mScenes[nName] = newScene;
                mOpenScene = nName;

                mEnt = {};
                mScene->clear();
                AssetManager::get()->destroy();
            }
        }
    }

    void Project::add(const std::string& path)
    {
        auto name = Utils::getFileName(path);
        auto found = mScenes.find(name);
        if (found == mScenes.end()) {
            auto newScene = "assets/scenes/" + name;
            mEnt = {};
            mScene->clear();
            AssetManager::get()->destroy();
            if (SceneLoader::loadFromPath(newScene.c_str(), mScene.get())) {
                mScenes[name] = newScene;
                mOpenScene = name;
            }
        }
    }

    void Project::Load(const std::string& path)
    {
        auto name = Utils::getFileName(path);
        auto found = mScenes.find(name);
        if (found != mScenes.end()) {

            mEnt = {};
            mScene->clear();
            AssetManager::get()->destroy();
            if (SceneLoader::loadFromPath(path.c_str(), mScene.get())) {
                found->second = path;
                std::replace(found->second.begin(), found->second.end(), '\\', '/');
                mOpenScene = name;
            }
        }
    }

    void Project::Save()
    {
        std::string json = Plutus::SceneSerializer(mScene.get());
        auto found = mScenes.find(mOpenScene);
        if (found != mScenes.end())
            saveBufferToFile(found->second, json.c_str());
    }

} // namespace Plutus