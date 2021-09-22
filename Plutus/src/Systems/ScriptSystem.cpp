#include "ScriptSystem.h"

#include <ECS/Scene.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/Script.h>

namespace Plutus
{
    ScriptSystem::ScriptSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera)
    {

    }

    void ScriptSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<Script>();

        for (auto [ent, script] : view.each()) {
            script.update(dt);
        }
    }
}