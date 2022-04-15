#include "SystemManager.h"
#include "Systems.h"

#include "System/System.h"

namespace Plutus
{

    SystemManager::SystemManager() {
        mSystems[&typeid(ScriptSystem)] = nullptr;
        mSystems[&typeid(PhysicSystem)] = nullptr;
        mSystems[&typeid(AnimationSystem)] = nullptr;
        mSystems[&typeid(RenderSystem)] = nullptr;
        mSystems[&typeid(DebugSystem)] = nullptr;
    }

    void SystemManager::init()
    {
        for (auto& sys : mSystems) {
            if (sys.second)sys.second->init(mScene);
        }
    }

    void SystemManager::stop()
    {
        for (auto& sys : mSystems) {
            if (sys.second) sys.second->destroy();
        }
    }

    void SystemManager::update(float dt)
    {
        for (auto& sys : mSystems) {
            if (sys.second)sys.second->update(dt);
        }
    }

    void SystemManager::cleanup()
    {
        for (auto& sys : mSystems) {
            if (sys.second) {
                sys.second->destroy();
                delete sys.second;
                sys.second = nullptr;
            }
        }
    }

} // namespace Plutus
