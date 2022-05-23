#include "SystemManager.h"

#include "Systems.h"

namespace Plutus
{

    SystemManager::SystemManager() {
        mSystems.resize(6);

        mSystems[getListId<ScriptSystem>()] = nullptr;
        mSystems[getListId<PhysicSystem>()] = nullptr;
        mSystems[getListId<AnimationSystem>()] = nullptr;
        mSystems[getListId<TileMapSystem>()] = nullptr;
        mSystems[getListId<RenderSystem>()] = nullptr;
        // mSystems[getListId<DebugSystem>()] = nullptr;
    }

    void SystemManager::init()
    {
        for (auto& sys : mSystems) {
            if (sys)sys->init(mProject);
        }
    }

    void SystemManager::stop()
    {
        for (auto& sys : mSystems) {
            if (sys)sys->destroy();
        }
    }

    void SystemManager::update(float dt)
    {
        for (auto& sys : mSystems) {
            if (sys)sys->update(dt);
        }
    }

    void SystemManager::cleanup()
    {
        for (size_t i = 0; i < mSystems.size(); i++)
        {
            if (mSystems[i]) {
                mSystems[i]->destroy();
                delete mSystems[i];
                mSystems[i] = nullptr;
            }
        }
    }

} // namespace Plutus
