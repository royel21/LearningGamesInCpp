#include "Config.h"
#include <Input/Input.h>
#include <Serialize/Serialize.h>
#include <rapidjson/document.h>

namespace Plutus
{
    AppConfig& AppConfig::get() {
        static AppConfig config;
        static bool isLoaded = false;
        if (!isLoaded) {
            config.load();
            Input::get()->onResize = [&](int w, int h) {
                config.winWidth = w;
                config.winHeight = h;
            };
        }
        return config;
    }

    AppConfig::~AppConfig() {
        save();
    }

    void AppConfig::load() {
        rapidjson::Document doc;
        if (loadJsonFromFile("Config.json", doc)) {

        }
    }

    void AppConfig::save() {
        Serializer ser;
        auto wr = ser.getWriter();
        wr->StartObject();
        {
            wr->String("WinWidth");
            wr->Int(Config.winWidth);
            wr->String("WinHeight");
            wr->Int(Config.winHeight);
            wr->String("Recentprojects");
            wr->StartArray();
            {
                for (auto& s : Config.recentProjects) {
                    wr->String(s.c_str());
                }
            }
            wr->EndArray();
        }
        wr->EndObject();
        saveJsonToFile("config.json", ser.getString());
    }

    AppConfig& Config = AppConfig::get();
} // namespace Plutus