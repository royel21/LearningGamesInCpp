#pragma once

namespace Plutus
{
    class MainGui
    {
    private:
        bool isInitialized = false;

    public:
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