#include "Project.h"

#include <ECS/Scene.h>
#include <Utils/FileIO.h>
#include <Serialize/Serialize.h>
#include <Assets/AssetManager.h>

#include <Serialize/SceneSerializer.h>
#include <Serialize/SceneLoader.h>

namespace Plutus
{
    Project::Project()
    {
        scene = CreateRef<Scene>();
    }

    void Project::clear()
    {
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
        currentScene = "";

        sceneList.clear();
    }

    void Project::load(const std::string& path)
    {
        rapidjson::Document doc;
        bool isLoaded = loadJsonFromFile(path.c_str(), doc);
        if (isLoaded) {
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
            //Create the List of scene for this project
            for (auto& scene : obj["scenes"].GetArray()) {
                sceneList[scene["name"].GetString()] = scene["path"].GetString();
            }

            currentScene.clear();

            if (!currentScene.empty())
                loadScene(sceneList[currentScene].c_str());
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
            ser.addFloat("zoom", zoomLevel);
            ser.addFloat("max-fps", maxFPS);

            ser.addInt("vel-iter", velIter);
            ser.addInt("pos-iter", positionIter);
            ser.add2Float("gravity", gravity);
            ser.addInt("auto-clear-force", autoClearForce);

            ser.addString("current-scene", currentScene);

            ser.StartArr("scenes");
            { for (auto& sc : sceneList) {
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
    }

    void Project::loadScene(const std::string& name)
    {
        auto found = sceneList.find(name);
        if (found != sceneList.end()) {
            scene->clear();
            SceneLoader::loadFromPath(found->second.c_str(), scene.get());
        }
    }

    void Project::saveScene()
    {
        if (currentScene.empty()) {
            auto data = SceneSerializer(scene.get());
            FileIO::saveBufferToFile(sceneList[currentScene].c_str(), data.c_str());
        }
    }
}