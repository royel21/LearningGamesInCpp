#include "Scene.h"
#include "Components.h"

#include <box2d/box2d.h>

namespace Plutus
{
    /*******************************     Scene    ************************/
    Scene::Scene() {
    }

    Scene::~Scene()
    {
        mRegistry.clear();
    }

    const std::string Entity::getName()
    {
        return getComponent<Tag>()->Name;
    }

    void Entity::setName(const std::string& name)
    {
        getComponent<Tag>()->Name = name;
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

    Entity Scene::getEntityByName(const std::string name)
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


    void Scene::removeEntity(entt::entity ent)
    {
        mRegistry.destroy(ent);
    }

    bool Scene::isValid(Entity ent) {
        return mRegistry.valid(ent);
    }

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

        CopyComponent<Transform>(source, dest);
        CopyComponent<Sprite>(source, dest);
        CopyComponent<Animation>(source, dest);
        CopyComponent<TileMap>(source, dest);
        CopyComponent<Script>(source, dest);
        CopyComponent<RigidBody>(source, dest);
    }

} // namespace Plutus
