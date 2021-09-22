#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <vector>
#include <cstring>
#include <unordered_map>
#include <Core/type.h>
#include <entt.hpp>
#include <algorithm>

namespace Plutus
{
    class Scene;

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

        Ref<Entity> add(Ref<Entity> item);

        void remove(const Entity* item);
    };

    using LayerMap = std::unordered_map<std::string, Layer>;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void removeEntity(Entity* ent);
        Entity* getEntity(uint32_t Id);
        Entity* createEntity(const std::string& name);
        entt::registry* getRegistry() { return &mRegistry; }

        bool removeLayer(std::string name);
        Layer* addNewLayer(const std::string& name);
        Layer* setCurrentLayer(const std::string& name);
        Layer* getCurrentLayer() { return mCurrentLayer; };
        LayerMap* getLayers() { return &mLayers; };

        void clear();

    private:
        entt::registry mRegistry;
        Layer* mCurrentLayer;

        LayerMap mLayers;
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