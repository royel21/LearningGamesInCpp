#pragma once
#include <Physics/Vectors.h>
#include <Systems/SystemManager.h>

#include "../ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    class CenterPanel {
    public:
        CenterPanel();
        void DrawCenterPanel();
        void update(float dt);

    private:
        void DrawViewPort();
        void CameraControl();
        void SelectEntity();

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