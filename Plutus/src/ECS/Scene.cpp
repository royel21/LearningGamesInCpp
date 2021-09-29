#include "Scene.h"
#include "./Components.h"

#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{
    Ref<Entity> Layer::add(Ref<Entity> item)
    {
        auto index = mEntities.size();
        mEntities.push_back(item);
        return mEntities[index];
    }

    void Layer::remove(const Entity* item)
    {
        auto e1 = std::remove_if(mEntities.begin(), mEntities.end(), [item](auto e) -> bool
            { return e->mId == item->mId; });

        if (e1 != mEntities.end())
        {
            mEntities.erase(e1, mEntities.end());
        }
    }
    /*******************************     Scene    ************************/
    Scene::Scene() {
        addNewLayer("Layer0");
    }

    Scene::~Scene()
    {
        mRegistry.clear();
        mLayers.clear();
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

    Entity* Scene::createEntity(const std::string& name)
    {
        auto ent = mCurrentLayer->add(CreateRef<Entity>(mRegistry.create(), this));
        ent->addComponent<Tag>(name);

        return ent.get();
    }

    void Scene::removeEntity(Entity* ent)
    {
        mRegistry.destroy(ent->mId);
        mCurrentLayer->remove(ent);
    }

    bool Scene::removeLayer(std::string name)
    {
        if (mLayers.size() == 1)
            return false;

        auto found = mLayers.find(name);
        if (found != mLayers.end())
        {
            for (int i = found->second.mEntities.size() - 1; i >= 0; i--)
            {
                removeEntity(found->second.mEntities[i].get());
            }
            mLayers.erase(found);
            return true;
        }
        return false;
    }

    Layer* Scene::addNewLayer(const std::string& name)
    {
        mLayers[name] = Layer(name);
        mCurrentLayer = &mLayers[name];
        return &mLayers[name];
    }

    Layer* Scene::setCurrentLayer(const std::string& name)
    {
        mCurrentLayer = &mLayers[name];
        return &mLayers[name];
    }

    void Scene::clear()
    {
        mLayers.clear();
        mRegistry.clear();
        addNewLayer("Layer0");
    }

    Entity* Scene::getEntity(uint32_t Id)
    {
        for (auto& ent : mCurrentLayer->mEntities) {
            if (ent->hasComponent<Transform>()) {
                if (entt::to_integral(ent->mId) == Id)
                    return ent.get();
            }
        }
        return nullptr;
    }

    Entity Scene::getEntity2(uint32_t Id)
    {
        auto ent = entt::entity(Id);
        return { mRegistry.valid(ent) ? ent : entt::null , this };
    }
} // namespace Plutus
