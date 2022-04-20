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
        if (!currentProject.empty()) {
            auto path = mProjects[currentProject];
            mProject.workingDir = Utils::getDirectory(path);
            AssetManager::get()->setBaseDir(mProject.workingDir);
            mProject.load(path);
        }
        mRender = render;
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

    EditorProject::EditorProject() : Project()
    {
        mTempScene = CreateRef<Scene>();
    }

    void EditorProject::Create(const std::string& name)
    {

    }

    void EditorProject::add(const std::string& path)
    {
        scene->clear();
        mTempScene->clear();
    }

} // namespace Plutus