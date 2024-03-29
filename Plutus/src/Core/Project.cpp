#include "Project.h"

#include <ECS/Scene.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>
#include <Serialize/Serialize.h>

#include <Assets/Assets.h>

#include <Serialize/SceneLoader.h>
#include <Serialize/SceneSerializer.h>

#include <Log/Logger.h>

namespace Plutus
{
    Project::Project()
    {
        scene = CreateRef<Scene>();
    }

    void Project::clear()
    {
        if (!currentScene.empty()) {
            winTitle = "Plutus App";
            winWidth = 1280;
            winHeight = 768;
            vpWidth = 1280;
            vpHeight = 768;
            vpPos = { 0, 0 };

            zoomLevel = 1.0f;
            maxFPS = 60.0f;
            velIter = 8;
            positionIter = 3;
            timeStepInSec = 1.0f / maxFPS;
            gravity = { 0.0f, -9.8f };
            autoClearForce = true;

            scene->clear();
            scenes.clear();
            currentScene = "";
            currentScenePath = "";
        }
    }

    void Project::load(const std::string& path)
    {
        std::string p = path;
#ifdef __EMSCRIPTEN__
        p = "assets/" + path;
#endif
        rapidjson::Document doc;

        if (loadJsonFromFile(p.c_str(), doc)) {
            Logger::info("load projected");
            isLoaded = true;
            clear();

#ifndef __EMSCRIPTEN__
            Plutus::AssetManager::get()->setBaseDir(Utils::getDirectory(path));
#endif

            JsonHelper jhelper;
            auto obj = doc.GetJsonObject();
            jhelper.value = &obj;

            winTitle = jhelper.getString("win-title");

            winWidth = jhelper.getInt("win-width", 1280);
            winHeight = jhelper.getInt("win-height", 768);

            vpWidth = jhelper.getInt("vp-width");
            vpHeight = jhelper.getInt("vp-height");

            vpPos = jhelper.getFloat2("vp-pos");

            zoomLevel = jhelper.getFloat("zoom-level");

            velIter = jhelper.getInt("velocity-iter");
            positionIter = jhelper.getInt("position-iter");

            maxFPS = jhelper.getFloat("max-fps");
            timeStepInSec = 1.0f / maxFPS;

            gravity = jhelper.getFloat2("gravity");

            autoClearForce = jhelper.getInt("auto-clear-force");

            currentScene = jhelper.getString("current-scene");

            if (doc.HasMember("scenes")) {
                for (auto& sc : doc["scenes"].GetArray()) {
                    scenes[sc["name"].GetString()] = sc["path"].GetString();
                }
            }
        }
    }

    void Project::save(const std::string& path)
    {
        Serializer ser;
        ser.StartObj();
        {
            ser.addString("win-title", winTitle);
            ser.addInt("win-width", winWidth);
            ser.addInt("win-height", winHeight);
            ser.addInt("vp-width", vpWidth);
            ser.addInt("vp-height", vpHeight);
            ser.add2Float("vp-pos", vpPos);
            ser.addFloat("zoom-level", zoomLevel);
            ser.addFloat("max-fps", maxFPS);

            ser.addInt("velocity-iter", velIter);
            ser.addInt("position-iter", positionIter);
            ser.add2Float("gravity", gravity);
            ser.addInt("auto-clear-force", autoClearForce);

            ser.addString("current-scene", currentScene);

            ser.StartArr("scenes");
            {
                for (auto sc : scenes) {
                    ser.StartObj();
                    {
                        ser.addString("name", sc.first);
                        ser.addString("path", sc.second);
                    }
                    ser.EndObj();
                }
            }
            ser.EndArr();
        }
        ser.EndObj();
        FileIO::saveBufferToFile(path, ser.getString());
        saveScene();
    }

    void Project::loadScene(const std::string& name)
    {
        if (name == "") {
            scene = CreateRef<Scene>();
        }
        else {
            auto tscene = scenes.find(name);
            if (tscene != scenes.end()) {
                scene->clear();
                currentScenePath = AssetManager::get()->getBaseDir() + tscene->second;
                SceneLoader::loadFromPath(tscene->second.c_str(), scene.get());
            }
        }
    }

    void Project::unLoadScene() {
        scene->clear();
        currentScene = "";
        currentScenePath = "";
    }

    void Project::saveScene()
    {
        if (!currentScene.empty()) {
            auto data = SceneSerializer(scene.get());
            FileIO::saveBufferToFile(currentScenePath.c_str(), data.c_str());
        }
    }
}