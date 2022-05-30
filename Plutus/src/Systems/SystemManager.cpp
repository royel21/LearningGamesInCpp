#include "SystemManager.h"

#include "Systems.h"

#include <Events/EventSystem.h>

namespace Plutus
{
    void SystemManager::init()
    {
        for (auto& sys : mSystems) {
            if (sys) {
                sys->setManager(this);
                sys->setProject(mProject);
                sys->init();
            }
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

        EventSystem::get()->cleanUp();
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
        EventSystem::get()->cleanUp();
    }

} // namespace Plutus
