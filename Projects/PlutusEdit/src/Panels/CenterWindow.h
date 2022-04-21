#pragma once
#include <Math/Vectors.h>
#include <Systems/SystemManager.h>

#include "../ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    struct Config;

    class CenterWindow {
    public:
        CenterWindow() = default;
        void init(Config* config);
        void draw();
        void update(float dt);

    private:
        void drawViewPort();
        void CameraControl();
        void selectEntity();
        vec2f getMousePos();
        void showConfig();

    private:
        Config* mConfig;

        vec2f mMouseLastCoords;
        vec2f mCamCoords;
        vec2f mEntLastPos;

        vec2f mCursor;
        vec2f mTexRegion;

        std::string currentScript;
        std::vector<std::string> scripts;
        TextEditor mTextEditor;


        SystemManager mSysManager;
    };

} // namespace Plutus