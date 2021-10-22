#pragma once
#include <ECS/Scene.h>


#include "TileMapPanel.h"
#include "AnimationTab.h"

namespace Plutus
{
    class ComponentPanel {
    public:
        void DrawComponentsPanel();

    private:
        void DrawTransform();
        void DrawSprite();
        void DrawScript();
    private:
        Entity mEnt;
        AnimationTab mAnimation;
        TileMapPanel mTileMapPanel;

        int uvIndex = 0;
        bool mUseCoords = false;
        float mSpriteScale = 1.0f;
    };
} // namespace Plutus