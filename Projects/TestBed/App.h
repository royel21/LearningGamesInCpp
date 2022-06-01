#pragma once
#include <vector>

#include <Core/Core.h>
#include <Math/Vectors.h>
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
        std::vector<uint32_t> mEntities;
        std::vector<Vec2f> mVertices;
        Vec2f mPoint;
        bool blocked = false;
    };
} //End of Plutus namespace