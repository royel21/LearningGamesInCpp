#pragma once
#include <Physics/Vectors.h>
#include "./ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    class CenterPanel {
    public:
        CenterPanel();
        void DrawCenterPanel();

    private:
        void DrawViewPort();
        void DrawViewPortControls();

    private:
        vec2f mMouseLastCoords;
        vec2f mCamCoords;
        std::string currentScript;
        std::vector<std::string> scripts;
        TextEditor editor;
    };

} // namespace Plutus