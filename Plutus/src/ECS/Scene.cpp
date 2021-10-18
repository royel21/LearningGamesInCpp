#include "Scene.h"
#include "Components.h"

namespace Plutus
{
    /*******************************     Scene    ************************/

    Scene::~Scene()
    {
        mRegistry.clear();
    }

    const std::string Entity::getName()
    {
        auto tag = getComponent<Tag>();
        return tag->Name;
    }
    void Entity::setName(const std::string& name)
    {
        auto tag = getComponent<Tag>();
        tag->Name = name;
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
        mRegistry.each([&](auto e) {
            auto& tag = mRegistry.get<Tag>(e);
            if (tag.Name.compare(name) == 0) {
                ent = e;
            }
            });

        return { ent, this };
    }

    void Scene::removeEntity(entt::entity ent)
    {
        mRegistry.destroy(ent);
    }

    bool Scene::isValid(Entity ent) {
        return mRegistry.valid(ent.mId);
    }

} // namespace Plutus
