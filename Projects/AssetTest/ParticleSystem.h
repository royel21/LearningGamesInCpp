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
    struct Texture;

    struct RenderableParticle {
        Vec2f pos;
        float size;
        ColorRGBA8 color;
        RenderableParticle(float x, float y, float s, ColorRGBA8 ucolor) : pos(x, y), size(s), color(ucolor) {}
        RenderableParticle(const Vec2f& p, float s, ColorRGBA8 ucolor) : pos(p), size(s), color(ucolor) {}
    };

    struct ParticleBatch {
        Texture* tex;
        std::vector<RenderableParticle> bufferVertices;
    };

    class ParticleSystem : public ISystem
    {
    public:
        ParticleSystem(Camera2D* camera) : ISystem(camera) {};
        ~ParticleSystem();
        void init(Project* project);
        void update(float dt);
        inline void setBlend(bool blend) { mBlend = blend; };

    private:
        Shader mShader;
        uint32_t mVBO;
        uint32_t mVAO;
        bool mBlend = true;

        std::unordered_map<int, ParticleBatch> mBatches;

        bool prepare(float dt);
    };
} // namespace Test