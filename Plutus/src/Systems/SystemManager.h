#pragma once

#include <vector>
#include <typeinfo>
#include <unordered_map>

namespace Plutus
{
    class Scene;
    class ISystem;

    class SystemManager
    {

    public:
        SystemManager();
        ~SystemManager() { cleanup(); };

        void setScene(Scene* scene) { mScene = scene; }

        void init();
        void stop();

        void update(float dt);
        void draw();

        template <typename T, typename... TArgs>
        T* AddSystem(TArgs &&... args)
        {

            T* newSystem = new T(std::forward<TArgs>(args)...);
            mSystems[&typeid(T)] = newSystem;
            return newSystem;
        }

        template <typename T>
        T* getSystem()
        {
            return hasSystem<T>() ? static_cast<T*>(it->second) : nullptr;
        }

        template <typename T>
        bool hasSystem()
        {
            return mSystems[&typeid(T)] != nullptr;
        }

        void cleanup();

    private:
        Scene* mScene;
        std::unordered_map<const std::type_info*, ISystem*> mSystems;
    };
} // namespace Plutus
