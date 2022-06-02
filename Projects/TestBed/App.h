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
        std::vector<uint32_t> mEntities;
        std::vector<Vec2f> mVertices;
        Vec2f mPoint;
        bool hit = false;
        bool usecircle = true;
        float mDist = 0;
        std::vector<Vec2f> mBuffer;

        uint32_t mVAO;
        uint32_t mBuffId;
        Shader mShader;

        void castRay(const Vec2f& Start, const Vec2f& end, float max, float angle = 0);
    };
} //End of Plutus namespace