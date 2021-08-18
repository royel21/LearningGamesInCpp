#pragma once

#include <memory>
#include <vector>
#include <cstring>
#include <entt.hpp>
#include <algorithm>
#include <unordered_map>
#include "Graphics/SpriteBatch2D.h"
#include "Graphics/Shader.h"
#include <Core/type.h>

namespace Plutus
{
    class Serializer;
    class Scene;
    class Camera2D;
    class Script;

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
        Layer() = default;
        Layer(const std::string& _name) : name(_name) {};
        std::vector<Ref<Entity>> mEntities;

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
        void Init(Camera2D* camera);
        void enableShader();
        void disableShader();

        Entity* createEntity(const std::string& name);
        void removeEntity(Entity* ent);

        std::unordered_map<std::string, Layer>* getLayers() { return &mLayers; };

        Layer* addLayer(const std::string& name);

        bool removeLayer(std::string name);

        Layer* setLayer(const std::string& name);
        Layer* getCurrentLayer() { return mCurrentLayer; };

        const entt::registry* getRegistry() { return &mRegistry; }

        void draw();
        void update(float dt);
        void serialize(Serializer& serializer);

        void clear();

        SpriteBatch2D* getRenderer() { return &mRenderer; }

        const glm::vec2 getScreen();

    private:
        entt::registry mRegistry;
        Shader mShader;
        SpriteBatch2D mRenderer;
        Camera2D* mCamera = nullptr;
        friend class Entity;
        Layer* mCurrentLayer;
        std::unordered_map<std::string, Layer> mLayers;
    };

    template <typename T>
    bool Entity::hasComponent()
    {
        return mScene->mRegistry.has<T>(mId);
    }

    template <typename T, typename... Args>
    T& Entity::addComponent(Args &&...args)
    {
        return !hasComponent<T>() ? mScene->mRegistry.emplace<T>(mId, std::forward<Args>(args)...) : getComponent<T>();
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

} // namespace Plutus