#include "ScriptSystem.h"

#include <ECS/Scene.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/Script.h>

namespace Plutus
{
    void ScriptSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<Script>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }
}