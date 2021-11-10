#include "SystemManager.h"
#include "Systems.h"

#include "System/System.h"

namespace Plutus
{

    void SystemManager::start()
    {
        for (auto& sys : mSystems) {
            sys->init();
        }
    }

    void SystemManager::stop()
    {
        for (auto& sys : mSystems) {
            sys->destroy();
        }
    }

    void SystemManager::update(float dt)
    {
        for (auto sys : mSystems) {
            sys->update(dt);
        }
    }

    void SystemManager::cleanup()
    {
        for (auto& sys : mSystems) {
            sys->destroy();
            delete sys;
        }
        mSystems.clear();
        mIndices.clear();
    }

} // namespace Plutus
