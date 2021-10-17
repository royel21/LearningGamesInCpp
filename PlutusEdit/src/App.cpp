#include "App.h"

#include "Helpers/Config.h"
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

    App::App(const char* name, int width, int height) : Core(name, Config.winWidth, Config.winHeight) {

    };

    App::~App() {

    }

    void App::Setup() {
        Render::get().Init();
        mMainGui.Init();
    }

    void App::Update(float dt) {
        Render::get().mCamera.update();
    }

    void App::Draw() {
        mMainGui.Begin();
        mCentralPanel.DrawCenterPanel();
        DrawScenes();
        AssetsTab.drawAssets();
        DrawComponentsPanel();
        mMainGui.End();

        Render::get().draw();
    }
    void App::Exit() {

    }
} // namespace Plutus
