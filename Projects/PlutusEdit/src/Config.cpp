#include "Config.h"

#include <filesystem>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Assets/AssetManager.h>

#include <ECS/Scene.h>

#include "Helpers/Render.h"
#include "Helpers/ConfigHelper.h"

#include <Platforms/Windows/FileUtils.h>
#include <Serialize/SceneSerializer.h>
#include "Helpers/data.h"

#include <Log/Logger.h>

namespace Plutus
{
    void createDirs(const std::string& workingDir) {
        auto dir = FileIO::joinPath(workingDir, "assets");
        if (!FileIO::exists(dir))
        {
            FileIO::mkdirs(FileIO::joinPath(dir, "fonts"));
            FileIO::mkdirs(FileIO::joinPath(dir, "scenes"));
            FileIO::mkdirs(FileIO::joinPath(dir, "script"));
            FileIO::mkdirs(FileIO::joinPath(dir, "sounds"));
            FileIO::mkdirs(FileIO::joinPath(dir, "textures"));
        }
    }

    Config::~Config() {
        save();
    }

    void Config::load() {
        loadConfig(this);
    }

    void Config::save() { SaveConfig(this); }

    void Config::init(Camera2D* camera)
    {
        if (!currentProject.empty()) {
            auto path = mProjects[currentProject];
            mProject.workingDir = Utils::getDirectory(path);
            AssetManager::get()->setBaseDir(mProject.workingDir);
            mProject.load(path);
            mProject.loadScene(mProject.currentScene);
        }
        mCamera = camera;
        mRender.mCamera = camera;
        mRender.init(this);
    }

    void Config::CreateProj()
    {
        std::string filePath;

        if (windowDialog(SAVE_FILE, filePath, "Select Directory")) {
            auto path = Utils::getDirectory(filePath);
            auto name = Utils::getFileName(filePath);

            createDirs(path);
            createProject(filePath.c_str());

            mProject.clear();
            mProject.workingDir = path;

            mProjects[name] = filePath;
            LoadProject(name);
        }
    }

    void Config::LoadProject(const std::string& name) {
        if (currentProject.compare(name) == 0) return;

        auto found = mProjects.find(name);
        if (found != mProjects.end()) {
            if (FileIO::exists(found->second)) {
                currentProject = name;
                mProject.workingDir = Utils::getDirectory(found->second);
                mProject.load(found->second);
                mProject.loadScene(mProject.currentScene);

                mRender.reload(this);
                SaveConfig(this);
            }
        }
    }

    void EditorProject::Copy(const EditorProject& proj)
    {
        winWidth = proj.winWidth;
        winHeight = proj.winHeight;
        vpWidth = proj.vpWidth;
        vpHeight = proj.vpHeight;
        vpPos = proj.vpPos;
        zoomLevel = proj.zoomLevel;
        maxFPS = proj.maxFPS;
        velIter = proj.velIter;
        positionIter = proj.positionIter;
        timeStepInSec = proj.timeStepInSec;
        gravity = proj.gravity;
        autoClearForce = proj.autoClearForce;

        scene->clear();
        scene->mBGColor = proj.scene->mBGColor;
        scene->copyScene(proj.scene.get());
    }

    std::string EditorProject::getDir(const std::string& part)
    {
        return workingDir + "assets\\" + Utils::ToLowerCase(part);
    }

    void EditorProject::loadSceneFromFile(const std::string& path)
    {
        auto ex = Utils::getExtension(path);

        if (FileIO::exists(path) && ex == "json") {
            auto name = Utils::getFileName(path);
            if (FileIO::copyFile(path, FileIO::joinPath(workingDir, "assets", "scenes", name))) {
                scenes[name] = "assets/scenes/" + name;
                loadScene(name);
            }
        }
    }

    bool EditorProject::CreateScene(const std::string& name)
    {
        auto savePath = workingDir + "assets\\scenes\\" + name + ".json";

        auto newName = name + ".json";
        auto fullPath = workingDir + "assets\\scenes\\" + newName;
        scenes[name] = "assets/scenes/" + newName;

        if (!FileIO::exists(savePath)) {
            FileIO::saveBufferToFile(fullPath, Data::EmptyScene);
        }
        loadScene(newName);

        return true;
    }

    void EditorProject::RenameScene(const std::string& oldName, const std::string& newName) {

        auto sc = scenes.find(oldName);

        auto dir = Utils::getDirectory(sc->second);

        auto oldPath = workingDir + sc->second;
        auto newPath = workingDir + dir + newName;
        if (FileIO::moveFile(oldPath, newPath)) {
            scenes[newName] = dir + newName;
            scenes.erase(oldName);
        }
    }

    void EditorProject::removeScene(std::string id)
    {
    }

    void EditorProject::clearScene()
    {
        scene->clear();
    }

} // namespace Plutus