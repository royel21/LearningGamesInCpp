#include "Config.h"
#include <Input/Input.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <iostream>
#include <fstream>  

#include <Serialize/Serialize.h>
#include <Serialize/SceneLoader.h>
#include <Serialize/SceneSerializer.h>

#include <Utils/Utils.h>
#include <Assets/AssetManager.h>
#include <Platforms/Windows/FileUtils.h>

namespace Plutus
{
    Project::Project()
    {
        mScene = CreateRef<Scene>();
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
                AssetManager::get()->clearData();
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
            AssetManager::get()->clearData();
            if (SceneLoader::loadFromJson(newScene.c_str(), mScene)) {
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
            AssetManager::get()->clearData();
            if (SceneLoader::loadFromJson(path.c_str(), mScene)) {
                found->second = path;
                std::replace(found->second.begin(), found->second.end(), '\\', '/');
                mOpenScene = name;
            }
        }
    }

    void Project::Save()
    {
        auto buffer = SceneSerializer(mScene);
        std::string json = Plutus::SceneSerializer(mScene);
        auto found = mScenes.find(mOpenScene);
        if (found != mScenes.end())
            toJsonFile(found->second, json.c_str());
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

    void Config::Init()
    {
        static bool isLoaded = false;
        if (!isLoaded) {
            load();
            Input::get()->onResize = [&](int w, int h) {
                winWidth = w;
                winHeight = h;
            };

            if (!std::filesystem::exists("assets"))
            {
                std::filesystem::create_directories("assets/textures");
                std::filesystem::create_directories("assets/audios");
                std::filesystem::create_directories("assets/fonts");
            }
        }

    }

    void Config::load() {
        PJson json("Config.json");
        if (json.isLoaded) {
            winWidth = json.getInt("win-width");
            winHeight = json.getInt("win-height");
            OpenProject = json.getString("open-project");
            for (auto& obj : json.doc["projects"].GetArray()) {
                auto& p = mProjects[obj["name"].GetString()];
                p.mOpenScene = obj["open-scene"].GetString();
                //ViewPoprt State
                {
                    auto vpstate = obj["vp-state"].GetJsonObject();
                    p.mVpState.width = vpstate["width"].GetInt();
                    p.mVpState.height = vpstate["height"].GetInt();
                }
                //List of scene
                for (auto& scene : obj["scenes"].GetArray()) {
                    p.mScenes[scene["name"].GetString()] = scene["path"].GetString();
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
    }

    bool Config::loadConfig()
    {
        PJson json("Config.json");
        if (json.isLoaded) {
            winWidth = json.getInt("win-width");
            winHeight = json.getInt("win-height");
        }

        return json.isLoaded;
    }

    void Config::save() {
        Serializer ser;
        ser.StartObj();
        {
            ser.addInt("win-width", winWidth);
            ser.addInt("win-height", winHeight);
            ser.addString("open-project", OpenProject);
            ser.StartArr("projects");
            {
                for (auto& p : mProjects) {
                    ser.StartObj();
                    {
                        ser.addString("name", p.first);
                        ser.addString("open-scene", p.second.mOpenScene);
                        ser.addString("vp-state");
                        ser.StartObj();
                        {
                            ser.addInt("width", p.second.mVpState.width);
                            ser.addInt("height", p.second.mVpState.height);
                        }
                        ser.EndObj();
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
} // namespace Plutus