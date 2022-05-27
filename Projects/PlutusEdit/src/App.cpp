#include "App.h"

#include <imgui.h>
#include "Config.h"
#include "Helpers/Render.h"
#include "Panels/SceneWindow.h"

#include <Serialize/Serialize.h>
#include <Systems/Systems.h>

// #include "./Panels/MianWindow.h"
#include "./Panels/AssetsWindow.h"
#include "./Panels/BottomWindow.h"
#include "./Panels/CenterWindow.h"
#include "./Panels/ComponentWindow.h"
#include "./Panels/SceneWindow.h"

#include <Graphics/DebugRenderer.h>

namespace Plutus
{
    App::App(const char* name, int width, int height) {
        mName = name;
        mWidth = width;
        mHeight = height;

        if (mConfig.isLoaded) {
            mWidth = mConfig.winWidth;
            mHeight = mConfig.winHeight;
        }
    };

    App::~App() {}

    void App::Init() {

        mConfig.init(&mCamera);

        mWinManager.addWindow<AssetsWindow>();
        mWinManager.addWindow<BottomWindow>();
        mWinManager.addWindow<CenterWindow>();
        mWinManager.addWindow<ComponentWindow>();
        mWinManager.addWindow<SceneWindow>();

        mWinManager.init(&mConfig, this);
    }

    void App::Update(float dt) {
        mWindow.setTitle(("Plutus Edit - " + mConfig.currentProject).c_str());
        mExit = mWinManager.mExit;
        mWinManager.update(dt);
    }

    void App::Dropfile(const char* file)
    {
        auto assetWin = mWinManager.getWindow<AssetsWindow>();
        if (assetWin) assetWin->fileDrop(file);
    }

    void App::Draw() {
        mWinManager.draw();
    }

    void App::Exit() {
    }
} // namespace Plutus
