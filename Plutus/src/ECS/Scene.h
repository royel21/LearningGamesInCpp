#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <vector>
#include <string>
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
        Entity(const Entity& ent) : mId(ent.mId), mScene(ent.mScene) { }
        Entity(const Entity* ent) : mId(ent->mId), mScene(ent->mScene) { }
        Entity(entt::entity ent, Scene* scene) : mId(ent), mScene(scene) {}

        entt::entity getEntityId() const { return mId; }

        const std::string getName();
        void setName(const std::string& name);

        operator bool() const { return mId != entt::null; }
        operator entt::entity() const { return mId; }
        operator uint32_t() const { return (uint32_t)mId; }

        bool operator==(const Entity& other) const
        {
            return mId == other.mId && mScene == other.mScene;
        }

        bool operator==(uint32_t id) const
        {
            return id == (uint32_t)mId;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

        bool operator!=(uint32_t id) const
        {
            return id != (uint32_t)mId;
        }

        template <typename T>
        bool hasComponent();

        template <typename T, typename... Args>
        T* addComponent(Args &&...args);

        template <typename T>
        T* getComponent();

        template <typename T>
        bool removeComponent();

    };

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name);

        Entity getEntity(int Id);
        Entity getEntityByName(const std::string name);
        void removeEntity(entt::entity ent);
        bool isValid(Entity ent);

        entt::registry* getRegistry() { return &mRegistry; }

        void clear() { mRegistry.clear(); }

    private:
        entt::registry mRegistry;
        friend class Entity;
        friend class System;
    };

    /**********************Entity definitions***************/
    template <typename T, typename... Args>
    T* Entity::addComponent(Args &&...args)
    {
        return &(mScene->mRegistry.emplace_or_replace<T>(mId, std::forward<Args>(args)...));
    }

    template <typename T>
    T* Entity::getComponent()
    {
        if (!hasComponent<T>()) return nullptr;
        return &(mScene->mRegistry.get<T>(mId));
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