#pragma once
#include <Math/Vectors.h>
#include <Systems/SystemManager.h>

#include "../ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    class CenterPanel {
    public:
        CenterPanel() = default;
        void init();
        void drawCenterPanel();
        void update(float dt);

    private:
        void drawViewPort();
        void CameraControl();
        void selectEntity();

    private:
        vec2f mMouseLastCoords;
        vec2f mCamCoords;
        vec2f mEntLastPos;
        std::string currentScript;
        std::vector<std::string> scripts;
        TextEditor editor;

        SystemManager mSysManager;
    };

} // namespace Plutus