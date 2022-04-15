#include "App.h"

#include <imgui.h>
#include "Config.h"
#include "Helpers/Render.h"
#include "Panels/SceneWindow.h"

#include <Serialize/Serialize.h>
#include <Systems/Systems.h>

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

    App::~App() {

    }

    void App::Init() {
        mConfig.init(&mRender);
        // mRender.init(&mConfig);

        mMainWin.init(&mConfig, this);
        mAssetsWin.init(&mConfig);
        mCenterWin.init(&mConfig);
        mCompWin.init(&mConfig);
        mConfigWin.init(&mConfig, &mRender);
        mSceneWindow.init(&mConfig, &mRender);

        // SceneWindow.init()
    }

    void App::Update(float dt) {
        mExit = mMainWin.mExit;
        mCenterWin.update(dt);
    }

    void App::Draw() {

        mMainWin.Begin();
        {
            mCenterWin.draw();
            if (mConfig.state != Running) {
                mAssetsWin.draw();
                mSceneWindow.draw();
                mCompWin.draw();
                mConfigWin.draw();
            }
        }
        mMainWin.End();

        mRender.draw();
    }
    void App::Exit() {

    }
} // namespace Plutus
