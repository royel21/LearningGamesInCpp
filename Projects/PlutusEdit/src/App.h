#pragma once
#include <Core/Core.h>
#include <Systems/SystemManager.h>

#include "Config.h"
#include "Helpers/Render.h"

#include "./Panels/MianWindow.h"
#include "./Panels/AssetsWindow.h"
#include "./Panels/BottomWindow.h"
#include "./Panels/CenterWindow.h"
#include "./Panels/ComponentWindow.h"
#include "./Panels/SceneWindow.h"


namespace Plutus
{
    class App : public Core
    {
    private:
        bool isInitialize = false;
        Config mConfig;
        Render mRender;

        MianWindow mMainWin;
        AssetsWindow mAssetsWin;
        CenterWindow mCenterWin;
        ComponentWindow mCompWin;
        BottomWindow mConfigWin;
        SceneWindow mSceneWindow;

    public:
        App() = default;
        App(const char* name, int width, int height);
        ~App();

        void initialize();

        void Init() override;
        void Update(float dt) override;
        void Dropfile(const char* file) override;
        void Draw() override;
        void Exit() override;
    };
} // namespace Plutus