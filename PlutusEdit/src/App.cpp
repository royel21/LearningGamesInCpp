#include "App.h"

#include "Config.h"
#include "Helpers/Render.h"
#include "Panels/ScenesList.h"
#include "Panels/ComponentsPanel.h"

#include <Serialize/Serialize.h>

#include <cstdio>

#include <imgui.h>

namespace Plutus
{
    App::App() {

    }

    App::App(const char* name, int width, int height) : Core(name, width, height) {
        auto& config = Config::get();
        if (config.isLoaded) {
            mWidth = config.winWidth;
            mHeight = config.winHeight;
        }
        else {
            mWidth = width;
            mHeight = height;
        }
        mName = name;

    };

    App::~App() {

    }

    void App::Setup() {
        Config::get().LoadProject("");
        Render::get().Init();
        mMainGui.Init();
    }

    void App::Update(float dt) {
        Render::get().mCamera.update();
        mExit = mMainGui.mExit;
    }

    void App::Draw() {
        mMainGui.Begin();
        mCentralPanel.DrawCenterPanel();
        DrawScenes();
        AssetsTab.drawAssets();
        mCompPanel.DrawComponentsPanel();
        mConfigPanel.DrawConfigPanel();
        mMainGui.End();

        Render::get().draw();
    }
    void App::Exit() {

    }
} // namespace Plutus
