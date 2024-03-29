#pragma once
#include <Math/Vectors.h>
#include <Systems/SystemManager.h>

#include "ImWindow.h"

#include "../ImGuiColorTextEdit/TextEditor.h"

namespace Plutus
{
    class CenterWindow : public ImWindow {
    public:
        CenterWindow() = default;
        void init(Config* config) override;
        void draw();
        void update(float dt);

    private:
        void drawViewPort();
        void CameraControl();
        void selectEntity(float x, float y);
        Vec2f getMousePos();
        void showConfig();

    private:
        Vec2f mMouseLastCoords;
        Vec2f mCamCoords;
        Vec2f mEntLastPos;

        Vec2f mCursor;
        Vec2f mTexRegion;

        std::string currentScript;
        std::vector<std::string> scripts;
        TextEditor mTextEditor;


        SystemManager mSysManager;
    };

} // namespace Plutus