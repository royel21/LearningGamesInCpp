#include "SystemManager.h"
#include "Systems.h"

#include "System/System.h"

namespace Plutus
{

    void SystemManager::start()
    {
        auto csSys = getSystem<ScriptSystem>();
        if (csSys) csSys->init();
        auto phSys = getSystem<PhysicSystem>();
        if (phSys) phSys->init();
        auto anSys = getSystem<AnimationSystem>();
        if (anSys) anSys->init();
        auto renSys = getSystem<RenderSystem>();
        if (renSys) renSys->init();
        auto debugSys = getSystem<DebugSystem>();
        if (debugSys) debugSys->init();
    }

    void SystemManager::stop()
    {
        for (auto& sys : mSystems) {
            sys.second->destroy();
        }
    }

    void SystemManager::update(float dt)
    {
        auto csSys = getSystem<ScriptSystem>();
        if (csSys) csSys->update(dt);
        auto phSys = getSystem<PhysicSystem>();
        if (phSys) phSys->update(dt);
        auto anSys = getSystem<AnimationSystem>();
        if (anSys) anSys->update(dt);
        auto renSys = getSystem<RenderSystem>();
        if (renSys) renSys->update(dt);
        auto debugSys = getSystem<DebugSystem>();
        if (debugSys) debugSys->update(dt);
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
