#include "Scene.h"
#include "Components.h"

namespace Plutus
{

    Vec2f Entity::getPosition() {
        auto trans = getComponent<TransformComponent>();
        if (trans) {
            return trans->getPosition();
        }

        return {};
    }

    const std::string Entity::getName()
    {
        return isValid() ? mScene->mRegistry.get<Tag>(mId).Name : "";
    }

    bool Entity::isValid() const {
        return mScene && mScene->mRegistry.valid(mId);
    }

    void Entity::setName(const std::string& name)
    {
        if (isValid())
            mScene->mRegistry.get<Tag>(mId).Name = name;
    }

    Entity Scene::createEntity(const std::string& name)
    {
        auto ent = Entity{ mRegistry.create(), this };
        ent.addComponent<Tag>(name);

        return { ent.mId, this };
    }

    Entity Scene::getEntity(int Id)
    {
        auto ent = entt::entity(Id);
        return { mRegistry.valid(ent) ? ent : entt::null , this };
    }

    Entity Scene::getEntityByName(const std::string& name)
    {
        entt::entity ent = entt::null;
        auto view = mRegistry.view<const Tag>();
        for (const auto& [e, tag] : view.each()) {
            if (tag.Name.compare(name) == 0) {
                ent = e;
                break;
            }
        }

        return { ent, this };
    }
    /*******************************     Scene    ************************/

    void Scene::copyScene(Scene* scene) {

        mRegistry.clear();
        scene->getRegistry()->each([&](auto& e) {
            Entity ent(e, scene);
            Entity newEnt = createEntity(ent.getName());
            CopyEntity(ent, newEnt);
            });
    }


    Entity Scene::CreateCopy(Entity& ent) {
        Entity newEnt = createEntity(ent.getName() + " copy");
        CopyEntity(ent, newEnt);
        return newEnt;
    }

    void CopyEntity(Entity& source, Entity& dest) {

        CopyComponent<TransformComponent>(source, dest);
        CopyComponent<SpriteComponent>(source, dest);
        CopyComponent<AnimationComponent>(source, dest);
        CopyComponent<TileMapComponent>(source, dest);
        CopyComponent<ScriptComponent>(source, dest);
        CopyComponent<RigidBodyComponent>(source, dest);
        CopyComponent<PhysicBodyComponent>(source, dest);
    }

} // namespace Plutus
