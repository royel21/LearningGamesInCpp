#pragma once
#include <Core.h>

#include "Panels/MainGui.h"
#include "Panels/AssetsTab.h"
#include "Panels/ConfigPanel.h"
#include "Panels/CenterPanel.h"
#include "Panels/ComponentPanels/ComponentsPanel.h"

#include <Systems/SystemManager.h>

namespace Plutus
{
    class App : public Core
    {
    private:
        bool isInitialize = false;
        MainGui mMainGui;
        AssetsTab AssetsTab;
        CenterPanel mCentralPanel;
        ComponentPanel mCompPanel;
        ConfigPanel mConfigPanel;

    public:
        App() = default;
        App(const char* name, int width, int height);
        ~App();

        void Setup() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;
    };
} // namespace Plutus