#pragma once
#include <string>

namespace Plutus
{
    class App;
    struct Config;

    class MianWindow
    {
    private:
        bool isInitialized = false;
        std::string toEdit;
        Config* mConfig;
        App* mApp;
        std::string mProjToRemove;

    public:
        bool mExit = false;
        ~MianWindow();
        void init(Config* config, App* app = nullptr);
        // Start Dear ImGui frame
        void Begin();
        // End Dear ImGui frame
        void End();

    private:
        void DockingWindow();
        void MenuGui(int flags);

    };
} // namespace Plutus