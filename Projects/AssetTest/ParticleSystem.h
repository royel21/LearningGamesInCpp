#pragma once

#include <Systems/System/System.h>
#include <Graphics/Shader.h>

#include <vector>
#include <Math/Vectors.h>
#include <unordered_map>
#include <stdint.h>

namespace Plutus
{
    class IndexBuffer;
    struct Project;

    struct RenderableParticle {
        Vec2f pos;
        Vec2f uv;
    };

    struct Batch {
        uint32_t offset = 0;
        uint32_t numVerts = 0;
        std::vector<RenderableParticle> bufferVertices;
    };

    class ParticleSystem : public ISystem
    {
        ParticleSystem();
        ~ParticleSystem();
        void init(Project* project);
        void update(float dt);

    private:
        Shader mShader;
        uint32_t mVBO;
        uint32_t mVAO;
        uint32_t mVertexCount = 0;
        uint32_t mIndexCount = 0;

        IndexBuffer* mIbo = nullptr;
        std::unordered_map<int, Batch> mBatches;

        void prepare(float dt);
    };
} // namespace Test