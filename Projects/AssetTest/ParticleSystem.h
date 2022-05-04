#pragma once

#include <Systems/System/System.h>
#include <Graphics/Shader.h>

#include <vector>
#include <Math/Vectors.h>
#include <unordered_map>
#include <stdint.h>

#include <Graphics/Camera2D.h>

namespace Plutus
{
    class IndexBuffer;
    struct Project;

    struct RenderableParticle {
        Vec2f pos;
        Vec2f uv;
        ColorRGBA8 color;
        RenderableParticle(float x, float y, float ux, float uy, ColorRGBA8 ucolor) : pos(x, y), uv(ux, uy), color(ucolor) {}
        RenderableParticle(const Vec2f& p, const Vec2f& up, ColorRGBA8 ucolor) : pos(p), uv(up), color(ucolor) {}
    };

    struct Batch {
        uint32_t buffSize = 0;
        uint32_t indexCount = 0;
        std::vector<RenderableParticle> bufferVertices;
    };

    class ParticleSystem : public ISystem
    {
    public:
        ParticleSystem(Camera2D* camera) : ISystem(camera) {};
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

        Camera2D mCamera;

        void prepare(float dt);
    };
} // namespace Test