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
            config->currentProject = jhelper.getString("current-project");

            //Create All Projects
            for (auto& jproject : doc["projects"].GetArray()) {
                config->mProjects[jproject["name"].GetString()] = jproject["path"].GetString();
            }
        }
    }

    void SaveConfig(Config* config)
    {
        Serializer ser;
        ser.StartObj();
        {
            ser.addInt("win-width", config->winWidth ? config->winWidth : 1280);
            ser.addInt("win-height", config->winHeight ? config->winHeight : 1280);
            ser.addString("current-project", config->currentProject);

            ser.StartArr("projects");
            for (auto& proj : config->mProjects) {
                ser.StartObj();
                {
                    ser.addString("name", proj.first);
                    ser.addString("path", proj.second);
                }
                ser.EndObj();
            }
            ser.EndArr();

            if (!config->currentProject.empty())
                config->mProject.save(config->mProjects[config->currentProject]);
        }
        ser.EndObj();

        FileIO::saveBufferToFile("config.json", ser.getString());
    }
}