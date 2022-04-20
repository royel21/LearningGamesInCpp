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

    void App::initialize()
    {
        if (!mConfig.currentProject.empty() && !isInitialize) {
            mConfig.init(&mRender);
            mAssetsWin.init(&mConfig);
            mCenterWin.init(&mConfig);
            mCompWin.init(&mConfig);
            mConfigWin.init(&mConfig, &mRender);
            mSceneWindow.init(&mConfig, &mRender);
            isInitialize = true;
        }

        isInitialize = !mConfig.currentProject.empty();
    }

    void App::Init() {
        mMainWin.init(&mConfig, this);
    }

    void App::Update(float dt) {
        initialize();

        mExit = mMainWin.mExit;
        if (isInitialize) {
            mCenterWin.update(dt);
        }
    }

    void App::Dropfile(const char* file)
    {
        if (isInitialize) {
            mAssetsWin.fileDrop(file);
        }
    }

    void App::Draw() {

        mMainWin.Begin();
        {
            if (isInitialize) {
                mCenterWin.draw();
                if (mConfig.state != Running) {
                    mAssetsWin.draw();
                    mSceneWindow.draw();
                    mCompWin.draw();
                    mConfigWin.draw();
                }
            }
        }

        mMainWin.End();
        if (isInitialize) {
            mRender.draw();
        }
    }
    void App::Exit() {

    }
} // namespace Plutus
