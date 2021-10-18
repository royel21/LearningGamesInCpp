#pragma once
#include "TileMapPanel.h"

namespace Plutus
{
    class ComponentPanel {
    public:
        void DrawComponentsPanel();

    private:
        void DrawTransform();
        void DrawSprite();
        void DrawAnimation();
        void DrawScript();
    private:
        TileMapPanel mTileMapPanel;
        float mSpriteScale = 1.0f;
    };
} // namespace Plutus