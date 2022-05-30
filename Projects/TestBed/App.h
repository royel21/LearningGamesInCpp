#pragma once
#include <Core/Core.h>
#include <Systems/SystemManager.h>

namespace Plutus
{
    class DebugRender;
    class PhysicSystem;

    class App : public Core
    {
    public:
        App();

        ~App();

        void Init() override;

        void Update(float dt) override;

        void Draw() override;

        void Exit() override;

    private:
        SystemManager mSysManager;
        DebugRender* mDbebug;
        PhysicSystem* mPhysicSys;
    };
} //End of Plutus namespace