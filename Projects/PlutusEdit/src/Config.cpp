#include "Config.h"

#include <filesystem>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Assets/AssetManager.h>

#include <ECS/Scene.h>

#include "Helpers/Render.h"
#include "Helpers/ConfigHelper.h"

#include <Platforms/Windows/FileUtils.h>

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

    void Config::init(Render* render)
    {
        mRender = render;
        if (!currentProject.empty()) {
            auto path = mProjects[currentProject];
            mProject.workingDir = Utils::getDirectory(path);
            AssetManager::get()->setBaseDir(mProject.workingDir);
            mProject.load(path);
        }
        mRender->init(this);
    }

    void Config::CreateProj()
    {
        std::string filePath;

        if (windowDialog(SAVE_FILE, filePath, "Select Directory")) {
            auto path = Utils::getDirectory(filePath);
            auto name = Utils::getFileName(filePath);

            createDirs(path);

            mProject.save(filePath);
            mProject.workingDir = path;

            currentProject = name;
            mProjects[name] = filePath;
        }
    }

    void Config::LoadProject(const std::string& name) {

        auto found = mProjects.find(name);
        if (found != mProjects.end()) {
            if (FileIO::exists(found->second)) {
                mProject.workingDir = Utils::getDirectory(found->second);
                mProject.load(found->second);
                mRender->reload(this);
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
        tileWidth = proj.tileWidth;
        tileHeight = proj.tileHeight;

        scene->clear();
        scene->mBGColor = proj.scene->mBGColor;
        scene->copyScene(proj.scene.get());
    }

    void EditorProject::CreateScene(const std::string& name)
    {

    }

    void EditorProject::removeScene(std::string id)
    {

    }

    void EditorProject::clearScene()
    {
        scene->clear();
    }

} // namespace Plutus