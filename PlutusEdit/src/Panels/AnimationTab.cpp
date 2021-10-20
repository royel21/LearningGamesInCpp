#include "AnimationTab.h"

#include <ECS/Scene.h>
#include <ECS/Components/Animation.h>

#include <Assets/AssetManager.h>

#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"

#include "ComponentUtil.h"

namespace Plutus
{
    void AnimationTab::DrawAnimation(Entity* ent)
    {
        mEnt = ent;
        mTestures = &AssetManager::get()->mTextures;
        if (CollapseComponent<Animation>("Animation##tilemap-comp", 4))
        {

        }
    }
}