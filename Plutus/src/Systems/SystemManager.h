#pragma once

#include "System.h"
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace Plutus
{
    class SystemManager
    {

    public:
        SystemManager() = default;
        ~SystemManager() { cleanup(); };
        void init(Scene *scene) { mScene = scene; }

        template <typename T>
        void AddSystem()
        {
            auto typeName = typeid(T).name();
            mSystems[typeName] = std::make_unique<T>(mScene);
        }
        void update(float dt)
        {
            for (auto &sys : mSystems)
            {
                sys.second->update(dt);
            }
        }
        void cleanup() { mSystems.clear(); }

    private:
        std::unordered_map<const char *, std::unique_ptr<ISystem>> mSystems;
        Scene *mScene;
    };
} // namespace Plutus
