#pragma once
#include <Core.h>

#include "Context.h"
#include "Panels/MainGui.h"
#include "Panels/AssetsTab.h"
#include "Panels/CenterPanel.h"

namespace Plutus
{
    class App : public Core
    {
    private:
        bool isInitialize = false;
        MainGui mMainGui;
        AssetsTab AssetsTab;
        CenterPanel mCentralPanel;

    public:
        App();
        App(const char* name, int width, int height);
        ~App();

        void Setup() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;
    };
} // namespace Plutus