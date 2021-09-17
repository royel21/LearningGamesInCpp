#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <vector>
#include <cstring>
#include <unordered_map>
#include <Core/type.h>
#include <entt.hpp>
#include <algorithm>
#include <Assets/Textures.h>

namespace Plutus
{
    class Serializer;
    class Scene;
    class Camera2D;
    class Script;
    class SpriteBatch2D;

    class Entity
    {
    public:
        entt::entity mId{ entt::null };
        Scene* mScene = nullptr;

    public:
        Entity() = default;
        Entity(entt::entity ent, Scene* scene) : mId(ent), mScene(scene) {}

        template <typename T>
        bool hasComponent();

        template <typename T, typename... Args>
        T& addComponent(Args &&...args);

        template <typename T>
        T& getComponent();

        template <typename T>
        bool removeComponent();

        entt::entity getEntityId() const { return mId; }

        const std::string getName();
        void setName(const std::string& name);
    };

    struct Layer
    {
        bool isVisible = true;
        std::string name;
        std::vector<Ref<Entity>> mEntities;

        Layer() = default;
        Layer(const std::string& _name) : name(_name) {};

        Ref<Entity> add(Ref<Entity> item)
        {
            auto index = mEntities.size();
            mEntities.push_back(item);
            return mEntities[index];
        }

        void remove(const Entity* item)
        {
            auto e1 = std::remove_if(mEntities.begin(), mEntities.end(), [item](auto e) -> bool
                { return e->mId == item->mId; });

            if (e1 != mEntities.end())
            {
                mEntities.erase(e1, mEntities.end());
            }
        }
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity* createEntity(const std::string& name);
        void removeEntity(Entity* ent);


        Layer* addLayer(const std::string& name);
        Layer* setLayer(const std::string& name);
        std::unordered_map<std::string, Layer>* getLayers() { return &mLayers; };
        Layer* getCurrentLayer() { return mCurrentLayer; };

        bool removeLayer(std::string name);

        const entt::registry* getRegistry() { return &mRegistry; }

        void draw(SpriteBatch2D* renderbatch);
        void update(float dt);
        void serialize(Serializer& serializer);

        void clear();

        Entity* getEntity(float x, float y);
        Entity* getEntity(uint32_t Id);
        void update();

    private:
        entt::registry mRegistry;
        Layer* mCurrentLayer;

        std::unordered_map<std::string, Layer> mLayers;
        friend class Entity;
        friend class System;
    };

    /**********************Entity definitions***************/
    template <typename T, typename... Args>
    T& Entity::addComponent(Args &&...args)
    {
        return mScene->mRegistry.emplace_or_replace<T>(mId, std::forward<Args>(args)...);
    }

    template <typename T>
    T& Entity::getComponent()
    {
        return mScene->mRegistry.get<T>(mId);
    }

    template <typename T>
    bool Entity::removeComponent()
    {
        if (hasComponent<T>())
        {
            mScene->mRegistry.remove<T>(mId);
        }
        return 1;
    }

    template <typename T>
    bool Entity::hasComponent()
    {
        return mScene->mRegistry.any_of<T>(mId);
    }
} // namespace Plutus