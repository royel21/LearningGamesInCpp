#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <string>
#include <entt.hpp>
#include <Math/Vectors.h>

class b2World;
class vec2f;

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
        bool isValid();
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
        Scene() = default;
        ~Scene() { mRegistry.clear(); }

        Entity createEntity(const std::string& name);

        Entity getEntity(int Id);
        Entity CreateCopy(Entity& ent);
        Entity getEntityByName(const std::string name);

        void copyScene(Scene* scene);
        inline void removeEntity(entt::entity ent) { mRegistry.destroy(ent); }
        inline bool isValid(Entity ent) { return mRegistry.valid(ent); }

        entt::registry* getRegistry() { return &mRegistry; }

        void clear() { mRegistry.clear(); }

        vec2f getGravity() { return mGravity; }

        void setGravity(const vec2f& g) { mGravity = g; }

        float getTimeIterSec() { return mTimeStepInSec; }
        void setTimeIterSec(float time) { mTimeStepInSec = time; }

        int getVelIter() { return mVelIter; }
        void setVelIter(int time) { mVelIter = time; }

        int getBodyIter() { return mBodyIter; }
        void setBodyIter(int time) { mBodyIter = time; }

        bool getAutoClearForce() { return mAutoClearForce; }
        void setAutoClearForce(bool val) { mAutoClearForce = val; }

    private:
        friend class Entity;
        friend class System;
        float mTimeStepInSec = 0.01667f;
        int mVelIter = 3;
        int mBodyIter = 6;
        bool mAutoClearForce = true;
        entt::registry mRegistry;
        vec2f mGravity = { 0.0f, -9.8f };
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