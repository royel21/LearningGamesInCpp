#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <typeinfo>

namespace Plutus
{
    class Scene;
    class ISystem;

    class SystemManager
    {

    public:
        SystemManager() = default;
        ~SystemManager() { cleanup(); };

        void setScene(Scene* scene) { mScene = scene; }

        void start();
        void stop();

        void update(float dt);
        void draw();

        template <typename T, typename... TArgs>
        T* AddSystem(TArgs &&... args)
        {
            if (!hasSystem<T>()) {
                T* newSystem = new T(mScene, std::forward<TArgs>(args)...);

                mSystems[&typeid(T)] = newSystem;
                return newSystem;
            }
            return getSystem<T>();
        }

        template <typename T>
        T* getSystem()
        {
            auto it = mSystems.find(&typeid(T));
            if (it != mSystems.end()) {
                return static_cast<T*>(it->second);
            }
            else {
                return nullptr;
            }
        }

        template <typename T>
        bool hasSystem()
        {
            return mSystems.find(&typeid(T)) != mSystems.end();
        }

        void cleanup();

    private:
        Scene* mScene;
        std::unordered_map<const std::type_info*, ISystem*> mSystems;

        friend class ISystem;
    };
} // namespace Plutus
