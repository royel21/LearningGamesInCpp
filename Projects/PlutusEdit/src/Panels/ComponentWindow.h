#pragma once
#include <ECS/Scene.h>

#include "ImWindow.h"

#include "./ComponentPanels/TileMapPanel.h"
#include "./ComponentPanels/AnimationTab.h"

namespace Plutus
{
    struct Config;

    class ComponentWindow : public ImWindow {
    private:
        Entity mEnt;
        AnimationTab mAnimation;
        TileMapPanel mTileMapPanel;

        int uvIndex = 0;
        bool mUseCoords = false;
        float mSpriteScale = 1.0f;
        float mTextColumnWidth = 0.30f;

    public:
        void draw();

    private:
        void DrawTransform();
        void DrawSprite();
        void DrawScript();
        void DrawRigidBody();
        void DrawPhysicBody();
    };
} // namespace Plutus