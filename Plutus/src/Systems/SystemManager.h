#pragma once

#include <memory>
#include <unordered_map>
#include <algorithm>
#include <typeinfo>

#include "System/System.h"

namespace Plutus
{
    class Scene;

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

                mSystems.push_back(newSystem);
                mIndices[&typeid(T)] = mSystems.size() - 1;
                return newSystem;
            }
            return getSystem<T>();
        }

        template <typename T>
        T* getSystem()
        {
            if (!hasSystem<T>()) return nullptr;

            return static_cast<T*>(mSystems[mIndices[&typeid(T)]]);
        }

        template <typename T>
        bool hasSystem()
        {
            return mIndices.find(&typeid(T)) != mIndices.end();
        }

        void cleanup();

    private:
        Scene* mScene;
        std::unordered_map<const std::type_info*, int> mIndices;
        std::vector<ISystem*> mSystems;

        friend class ISystem;
    };
} // namespace Plutus
