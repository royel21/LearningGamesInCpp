#include "ConfigHelper.h"

#include "../Config.h"

#include <Utils/FileIO.h>
#include <rapidjson/document.h>
#include <Serialize/Serialize.h>

namespace Plutus
{
    void loadConfig(Config* config)
    {
        rapidjson::Document doc;

        bool isLoaded = loadJsonFromFile("Config.json", doc);

        if (isLoaded) {
            JsonHelper jhelper;
            auto obj = doc.GetJsonObject();
            jhelper.value = &obj;

            config->winWidth = jhelper.getInt("win-width", 1280);
            config->winHeight = jhelper.getInt("win-height", 768);
            config->OpenProject = jhelper.getString("open-project");

            //Create All Projects
            for (auto& jproject : doc["projects"].GetArray()) {

                auto& project = config->mProjects[jproject["name"].GetString()];
                project.mConfig = config;

                project.mOpenScene = jproject["open-scene"].GetString();
                project.vpWidth = jproject["width"].GetInt();
                project.vpHeight = jproject["height"].GetInt();

                //Create the List of scene for this project
                for (auto& scene : jproject["scenes"].GetArray()) {
                    project.mScenes[scene["name"].GetString()] = scene["path"].GetString();
                }
            }
        }

        if (config->OpenProject.empty()) {
            config->mProject = &config->mProjects["Project1"];
            config->OpenProject = "Project1";
        }
        else {
            config->mProject = &config->mProjects[config->OpenProject];
        }
    }

    void SaveConfig(Config* config)
    {
        Serializer ser;
        ser.StartObj();
        {
            ser.addInt("win-width", config->winWidth ? config->winWidth : 1280);
            ser.addInt("win-height", config->winHeight ? config->winHeight : 1280);
            ser.addString("open-project", config->OpenProject);
            ser.StartArr("projects");
            {
                for (auto& p : config->mProjects) {
                    ser.StartObj();
                    {
                        ser.addString("name", p.first);
                        ser.addString("open-scene", p.second.mOpenScene);
                        ser.addInt("win-width", p.second.windowWidth);
                        ser.addInt("win-height", p.second.windowHeight);
                        ser.addInt("vp-width", p.second.vpWidth);
                        ser.addInt("vp-height", p.second.vpHeight);
                        ser.addFloat("zoom", p.second.zoomLevel);

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

        saveBufferToFile("config.json", ser.getString());
    }
}