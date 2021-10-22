#include "SystemManager.h"

namespace Plutus
{

    void SystemManager::start()
    {
        for (auto& sys : mSystems) {
            sys.second->init();
        }
    }

    void SystemManager::stop()
    {
        for (auto& sys : mSystems) {
            sys.second->destroy();
        }
    }

    void SystemManager::update(float dt)
    {
        for (auto sys : mSystems) {
            sys.second->update(dt);
        }
    }

    void SystemManager::draw()
    {

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
