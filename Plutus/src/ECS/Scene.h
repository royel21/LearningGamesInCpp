#pragma once
#pragma warning(disable : 4307) //disable warning integral constant overflow for entt

#include <vector>
#include <cstring>
#include <unordered_map>
#include <Graphics/SpriteBatch2D.h>
#include <Graphics/Shader.h>
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
        Scene() = default;
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

        Camera2D* getCamera() { return mCamera; }
        Shader* getShader() { return &mShader; }

    private:
        entt::registry mRegistry;
        SpriteBatch2D mRenderer;
        Shader mShader;

        Camera2D* mCamera = nullptr;
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