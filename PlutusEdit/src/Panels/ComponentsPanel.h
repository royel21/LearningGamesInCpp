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
        TileMapPanel mTileMapPanel;
        AnimationTab mAnimation;
        float mSpriteScale = 1.0f;
        Entity mEnt;
    };
} // namespace Plutus