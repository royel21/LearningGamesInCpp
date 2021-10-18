#pragma once
#include <string>

namespace Plutus
{
    class MainGui
    {
    private:
        bool isInitialized = false;
        std::string toEdit;

    public:
        bool mExit = false;

        MainGui() = default;
        ~MainGui();
        void Init();
        // Start Dear ImGui frame
        void Begin();
        // End Dear ImGui frame
        void End();

    private:
        void DockingWindow();
        void MenuGui(int flags);
    };
} // namespace Plutus