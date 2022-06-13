#pragma once
#include <Systems/System/System.h>

#include <vector>
#include <cstdint>

#include <Math/Vectors.h>
#include <Graphics/Shader.h>


namespace Plutus
{
    class ShapeSystem : public ISystem
    {
    public:
        ShapeSystem(Camera2D* camera) : ISystem(camera) {};
        ~ShapeSystem();
        void init() override;

        void update(float dt) override;

    private:
        Shader mShader;
        uint32_t mVAOId;
        uint32_t mBuffId;
        uint32_t mIBuffId;
        std::vector<Vec2f> mPoints;
        std::vector<uint32_t> mIndices;

        void prepare();
        void createIndex(uint32_t count);
    };
}// End Plutus namespace