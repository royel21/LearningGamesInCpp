#include "Scene.h"
#include "./Components.h"

#include <Graphics/SpriteBatch2D.h>

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
        return tag.Name;
    }
    void Entity::setName(const std::string& name)
    {
        auto tag = getComponent<Tag>();
        tag.Name = name;
    }

    Entity Scene::createEntity(const std::string& name)
    {
        auto ent = Entity{ mRegistry.create(), this };
        ent.addComponent<Tag>(name);

        return { ent.mId, this };
    }

    Entity Scene::getEntity(uint32_t Id)
    {
        auto ent = entt::entity(Id);
        return { mRegistry.valid(ent) ? ent : entt::null , this };
    }

    void Scene::removeEntity(entt::entity ent)
    {
        mRegistry.destroy(ent);
    }


} // namespace Plutus
