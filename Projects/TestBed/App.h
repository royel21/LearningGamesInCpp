#pragma once
#include <vector>

#include <Core/Core.h>
#include <Math/Vectors.h>
#include <Systems/SystemManager.h>
#include <Graphics/Shader.h>

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
        std::vector<Vec2f> mVertices;
        Vec2f mPoint;
        Vec2f mOrgVec;
        bool hit = false;
        float mDist = 0;
        std::vector<std::tuple<float, Vec2f>> points;

        uint32_t mVAO;
        uint32_t mBuffId;
        Shader mShader;
    };
} //End of Plutus namespace