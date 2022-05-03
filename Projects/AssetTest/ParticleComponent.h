#pragma once

#include <vector>
#include <string>
#include <functional>

#include <Math/Vectors.h>

class b2Body;

namespace Plutus
{
    struct Texture;

    struct Particle
    {
        Vec2f position;
        Vec2f velocity = { 1 };
        int size = 1;
        float lifeTime = 0;
        int texCoord = 0;

        Particle() = default;
        Particle(const Vec2f& pos, int _size, const Vec2f& vel, float _lifeTime, int _texCoord = 0)
            : position(pos), velocity(vel), size(_size), lifeTime(_lifeTime), texCoord(_texCoord) {};

        operator bool() { return lifeTime < 0; }
    };

    struct ParticleComponent
    {
        Vec2f offset;
        float frameTime = 0;
        int mMaxCount = 0;
        int lastIndex = 0;

        uint32_t mIndexCount = 0;

        std::string mTexId;
        std::vector<int> mFrames;
        Texture* mTexture = nullptr;
        std::vector<Particle> mParticles;
        std::function<void(Particle&, float)> mUpdate = nullptr;

        ParticleComponent();
        ParticleComponent(int maxCount, const std::string& texId = "", std::function<void(Particle&, float)> updatefunc = nullptr) { init(maxCount, texId, updatefunc); }

        void init(int maxCount, const std::string& texId = "", std::function<void(Particle&, float)> updatefunc = nullptr);

        inline void addParticle(const Vec2f& pos, int size, const Vec2f& vel, float lifeTime, int texCoord = 0) {

            mParticles[lastIndex] = { pos, size, vel, lifeTime, texCoord };
            lastIndex = (lastIndex + 1) % mMaxCount;
        }

    private:
        float elapseTime = 0;
    };
} // namespace Plutus