#include "Config.h"
#include <Input/Input.h>
#include <Serialize/Serialize.h>
#include <rapidjson/document.h>
#include <filesystem>
#include <iostream>
#include <fstream>  

#include <Utils/Utils.h>

namespace Plutus
{
    void AppConfig::CreateProj(const char* filePath)
    {

        std::ofstream outfile(filePath);

        outfile << "{ \"scenes\": {} }" << std::endl;

        outfile.close();
        Projects.insert(Projects.begin(), filePath);
        CurrentProject = filePath;
        project.clear();
    }

    void AppConfig::LoadProject(const char* projPath)
    {
        rapidjson::Document doc;
        if (loadJsonFromFile(projPath, doc) && doc.HasMember("scenes")) {
            auto scenes = doc["scenes"].GetJsonObject();
            for (auto itr = scenes.MemberBegin(); itr != scenes.MemberEnd(); ++itr) {
                project[itr->name.GetString()] = itr->value.GetString();
            }

            int index = Utils::getIndex(Projects, projPath);
            if (index > -1)
                Projects.erase(Projects.begin() + index);
            Projects.insert(Projects.begin(), projPath);
            CurrentProject = projPath;
        }
    }

    AppConfig& AppConfig::get() {
        static AppConfig config;
        static bool isLoaded = false;
        if (!isLoaded) {
            config.load();
            Input::get()->onResize = [&](int w, int h) {
                config.winWidth = w;
                config.winHeight = h;
            };

            if (!std::filesystem::exists("assets"))
            {
                std::filesystem::create_directories("assets/textures");
                std::filesystem::create_directories("assets/audios");
                std::filesystem::create_directories("assets/fonts");
                std::filesystem::create_directories("projects");
            }
        }
        return config;
    }

    AppConfig::~AppConfig() {
        save();
    }

    void AppConfig::load() {
        PJson json("Config.json");
        if (json.isLoaded) {
            winWidth = json.getInt("WinWidth");
            winHeight = json.getInt("WinHeight");
            CurrentProject = json.getString("CurrentProject");
            for (auto& v : json.doc["Projects"].GetArray()) {
                Projects.push_back(v.GetString());
            }

            if (!CurrentProject.empty() && std::filesystem::exists(CurrentProject)) {
                LoadProject(CurrentProject.c_str());
            }
        }
    }

    void AppConfig::save() {
        Serializer ser;
        ser.StartObj();
        {
            ser.addInt("WinWidth", Config.winWidth);
            ser.addInt("WinHeight", Config.winHeight);
            ser.addString("CurrentProject", Config.CurrentProject.c_str());
            ser.StartArr("Projects");
            {
                for (auto& s : Config.Projects) {
                    ser.addString(s.c_str());
                }
            }
            ser.EndArr();
        }
        ser.EndObj();

        saveJsonToFile("config.json", ser.getString());
    }

    AppConfig& Config = AppConfig::get();
} // namespace Plutus