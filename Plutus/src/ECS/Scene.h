#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <string>
#include <entt.hpp>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

class b2World;

namespace Plutus
{
    class Scene;
    struct TransformComponent;

    class Entity
    {
    public:
        entt::entity mId{ entt::null };
        Scene* mScene = nullptr;
        bool mVisible = true;

    public:
        Entity() = default;
        Entity(const Entity& ent) : mId(ent.mId), mScene(ent.mScene) { }
        Entity(const Entity* ent) : mId(ent->mId), mScene(ent->mScene) { }
        Entity(entt::entity ent, Scene* scene) : mId(ent), mScene(scene) {}

        uint32_t getId() { return (uint32_t)mId; }

        entt::entity getEntityId() const { return mId; }

        const std::string getName();
        Vec2f getPosition();
        Vec2f getCenter();
        Vec2f getDirection(Entity& Ent);
        float getDistance(Entity& Ent);


        void setName(const std::string& name);
        bool isValid() const;
        operator bool() const { return isValid(); }
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
        ColorRGBA8 mBGColor;

        ~Scene() { mRegistry.clear(); }

        Entity createEntity(const std::string& name, bool visible = true);

        Entity getEntity(int Id);
        Entity CreateCopy(Entity& ent);
        Entity getEntityByName(const std::string& name);

        inline TransformComponent* getTransform(uint32_t id) { return getTransform(entt::entity(id)); }
        TransformComponent* getTransform(entt::entity id);

        template<typename T>
        T* getComponentFromName(const std::string& name) {
            auto ent = getEntityByName(name);
            if (ent) return &(mRegistry.get<T>(ent.mId));
            return nullptr;
        }

        template<typename T>
        T* getComponent(uint32_t id) {
            return getComponent<T>(entt::entity(id));
        }

        template<typename T>
        T* getComponent(entt::entity id) {
            if (mRegistry.valid(id) && mRegistry.any_of<T>(id)) {
                return  &(mRegistry.get<T>(id));
            }
            return nullptr;
        }

        void copyScene(Scene* scene);

        inline void removeEntity(entt::entity ent) { mRegistry.destroy(ent); }

        inline bool isValid(Entity ent) { return mRegistry.valid(ent); }
        inline bool isValid(uint32_t ent) { return mRegistry.valid(entt::entity(ent)); }

        inline entt::registry* getRegistry() { return &mRegistry; }
        // remove all entity from scene
        inline void clear() { mRegistry.clear(); mBGColor = { 255,255,255,255 }; }

        template<typename... T>
        inline auto getView() {
            return mRegistry.view<T...>();
        }

        template<typename... T>
        inline auto getGroup() {
            return mRegistry.group<T...>();
        }

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
        if (isValid() && hasComponent<T>())
        {
            mScene->mRegistry.remove<T>(mId);
        }
        return 1;
    }

    template <typename T>
    bool Entity::hasComponent()
    {
        return isValid() && mScene->mRegistry.any_of<T>(mId);
    }

    template<typename T>
    void CopyComponent(Entity& ent1, Entity& ent2) {
        if (ent1.hasComponent<T>()) {
            ent2.addComponent<T>(*ent1.getComponent<T>());
        }
    }

    template<typename ...Component>
    void CopyComponents(Entity& ent1, Entity& ent2) {
        (CopyComponent<Component>(ent1, ent2), ...);
    }

    void CopyEntity(Entity& source, Entity& dest);
} // namespace Plutus