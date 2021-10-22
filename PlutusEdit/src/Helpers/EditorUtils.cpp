#include "EditorUtils.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

namespace Plutus
{
    template<typename T>
    void CopyComponent(Entity& ent1, Entity& ent2) {
        if (ent1.hasComponent<T>()) {
            ent2.addComponent<T>(*ent1.getComponent<T>());
        }
    }

    void CopyScene(Scene* fromScene, Scene* toScene)
    {
        fromScene->getRegistry()->each([&](auto& e) {
            Entity ent(e, fromScene);
            Entity newEnt = toScene->createEntity(ent.getName());

            CopyComponent<Transform>(ent, newEnt);
            CopyComponent<Sprite>(ent, newEnt);
            CopyComponent<Animation>(ent, newEnt);
            CopyComponent<TileMap>(ent, newEnt);
            CopyComponent<Script>(ent, newEnt);
            });
    }
}