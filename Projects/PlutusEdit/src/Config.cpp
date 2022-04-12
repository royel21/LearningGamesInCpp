#include "Config.h"
#include <rapidjson/document.h>

#include <math.h>
#include <fstream>  
#include <iostream>
#include <filesystem>

#include <Serialize/Serialize.h>
#include <Serialize/SceneLoader.h>
#include <Serialize/SceneSerializer.h>

#include <Utils/Utils.h>
#include <Assets/AssetManager.h>
#include <Platforms/Windows/FileUtils.h>


namespace Plutus
{

    void Config::Init()
    {
        if (!isLoaded) {
            if (!std::filesystem::exists("assets"))
            {
                std::filesystem::create_directories("assets/textures");
                std::filesystem::create_directories("assets/audios");
                std::filesystem::create_directories("assets/fonts");
            }
            isLoaded = true;
        }

    }

    void Config::CreateProj(const char* name)
    {
        mProject = &mProjects[name];
        OpenProject = name;
    }

    Config& Config::get() {
        static Config config;
        return config;
    }

    Config::~Config() {
        save();
    }

    void Config::RenameProj(const std::string& oldname, const std::string newName) {
        mProjects[newName] = mProjects[oldname];
        mProjects.erase(oldname);
        if (OpenProject.compare(oldname) == 0) {
            mProject = &mProjects[newName];
            OpenProject = newName;
        }
    }

    void Config::LoadProject(const std::string& name) {
        if (name.empty()) {
            load();
            if (mProject && mProject->mScenes.size())
                mProject->Load(mProject->mScenes[mProject->mOpenScene]);
        }
        else {
            mProjects[name] = Project();
            mProject = &mProjects[name];
        }

    }

    void Config::load() {
        rapidjson::Document doc;

        bool isLoaded = loadJsonFromFile("Config.json", doc);

        if (isLoaded) {
            JsonHelper jhelper;
            jhelper.value = &doc.GetJsonObject();

            winWidth = jhelper.getInt("win-width", 1280);
            winHeight = jhelper.getInt("win-height", 768);
            OpenProject = jhelper.getString("open-project");
            vpZoom = jhelper.getFloat("vp-zoom", 1);
            vpPos = jhelper.getFloat2("vp-pos");
            vpColor = jhelper.getFloat4("vp-color", { 1,1,1,1 });

            for (auto& obj : doc["projects"].GetArray()) {

                auto& p = mProjects[obj["name"].GetString()];
                p.mOpenScene = obj["open-scene"].GetString();
                p.vpWidth = obj["width"].GetInt();
                p.vpHeight = obj["height"].GetInt();
                //List of scene
                for (auto& scene : obj["scenes"].GetArray()) {
                    p.mScenes[scene["name"].GetString()] = scene["path"].GetString();
                }
            }
        }

        if (OpenProject.empty()) {
            mProject = &mProjects["Project1"];
            OpenProject = "Project1";
        }
        else {
            mProject = &mProjects[OpenProject];
        }
    }

    void Config::save() {
        Serializer ser;
        ser.StartObj();
        {
            ser.addInt("win-width", winWidth ? winWidth : 1280);
            ser.addInt("win-height", winHeight ? winHeight : 1280);
            ser.addString("open-project", OpenProject);
            ser.addFloat("vp-zoom", roundf(vpZoom * 100) / 100);
            ser.StartArr("vp-pos");
            {
                ser.addFloat(vpPos.x);
                ser.addFloat(vpPos.y);
            }
            ser.EndArr();
            ser.StartArr("vp-color");
            {
                ser.addFloat(roundf(vpColor.x * 100) / 100);
                ser.addFloat(roundf(vpColor.y * 100) / 100);
                ser.addFloat(roundf(vpColor.z * 100) / 100);
                ser.addFloat(roundf(vpColor.w * 100) / 100);
            }
            ser.EndArr();
            ser.StartArr("projects");
            {
                for (auto& p : mProjects) {
                    ser.StartObj();
                    {
                        ser.addString("name", p.first);
                        ser.addString("open-scene", p.second.mOpenScene);
                        ser.addInt("width", p.second.vpWidth);
                        ser.addInt("height", p.second.vpHeight);
                        ser.StartArr("scenes");
                        { for (auto& p : p.second.mScenes) {
                            ser.StartObj();
                            {
                                ser.addString("name", p.first);
                                ser.addString("path", p.second);
                            }
                            ser.EndObj();
                        }
                        }
                        ser.EndArr();
                    }
                    ser.EndObj();

                }
            }
            ser.EndArr();
        }
        ser.EndObj();

        saveJsonToFile("config.json", ser.getString());
    }

    Project::Project()
    {
        mScene = CreateRef<Scene>();
        mTempScene = CreateRef<Scene>();
    }

    void Project::Create(const std::string& name)
    {
        auto nName = name + ".json";
        auto found = Config::get().mProject->mScenes.find(name);

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
            toJsonFile(found->second, json.c_str());
    }
} // namespace Plutus