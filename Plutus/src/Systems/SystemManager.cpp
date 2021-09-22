#include "SystemManager.h"

namespace Plutus
{
    void SystemManager::update(float dt)
    {
        for (auto sys : mSystems) {
            sys.second->update(dt);
        }
    }

    void SystemManager::cleanup()
    {
        for (auto& sys : mSystems) {
            sys.second->destroy();
            delete sys.second;
        }
        mSystems.clear();
    }


} // namespace Plutus
