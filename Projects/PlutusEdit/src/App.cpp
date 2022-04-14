#include "App.h"

#include <imgui.h>
#include "Config.h"
#include "Helpers/Render.h"
#include "Panels/ScenesList.h"

#include <Serialize/Serialize.h>
#include <Systems/Systems.h>

namespace Plutus
{
    App::App(const char* name, int width, int height) {
        mName = name;
        mWidth = width;
        mHeight = height;

        auto& config = Config::get();
        if (config.isLoaded) {
            mWidth = config.winWidth;
            mHeight = config.winHeight;
        }
    };

    App::~App() {

    }

    void App::Init() {
        Config::get().LoadProject("");
        Render::get().Init();
        mMainGui.Init();
        Render::get().setScene(Config::get().mProject->mScene.get());
        mCentralPanel.init();
    }

    void App::Update(float dt) {
        mExit = mMainGui.mExit;
        mCentralPanel.update(dt);
    }

    void App::Draw() {
        mMainGui.Begin();
        mCentralPanel.drawCenterPanel();
        if (Config::get().state != Running) {
            AssetsTab.drawAssets();
            DrawScenes();
            mCompPanel.DrawComponentsPanel();
            mConfigPanel.DrawConfigPanel();
        }
        mMainGui.End();

        Render::get().draw();
    }
    void App::Exit() {

    }
} // namespace Plutus
