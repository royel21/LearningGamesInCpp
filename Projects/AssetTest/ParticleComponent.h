#pragma once

#include <vector>
#include <string>
#include <functional>

#include <Math/Vectors.h>

#include <Utils/ColorRGBA8.h>

class b2Body;

namespace Plutus
{
    class ParticleSystem;

    struct Texture;

    struct Particle
    {
        Vec2f position;
        Vec2f velocity = { 1 };
        int size = 1;
        float lifeTime = 0;
        float elapse = 0;
        int texCoord = 0;

        Particle() = default;
        Particle(const Vec2f& pos, int _size, const Vec2f& vel, float _lifeTime, int _texCoord = 0)
            : position(pos), velocity(vel), size(_size), lifeTime(_lifeTime), texCoord(_texCoord) {};

        operator bool() { return elapse < lifeTime; }
    };

    struct ParticleComponent
    {
        Vec2f offset;
        float frameTime = 0;
        int mMaxCount = 0;
        int lastIndex = 0;
        ColorRGBA8 color;

        uint32_t mIndexCount = 0;

        std::vector<Particle> mParticles;
        std::function<float(Particle&, float)> mUpdate = nullptr;

        ParticleComponent();
        /**
         * @brief Construct a new Particle Component object
         *
         * @param maxCount
         * @param texId
         * @param updatefunc
         */
        ParticleComponent(int maxCount, const std::string& texId = "", std::function<float(Particle&, float)> updatefunc = nullptr) { init(maxCount, texId, updatefunc); }

        void init(int maxCount, const std::string& texId = "", std::function<float(Particle&, float)> updatefunc = nullptr);

        inline void addParticle(const Vec2f& pos, int size, const Vec2f& vel, float lifeTime, int texCoord = 0) {
            auto& p = mParticles[lastIndex];
            if (!p) {
                count++;
                mParticles[lastIndex] = { pos + offset, size, vel, lifeTime, texCoord };
            }
            lastIndex = (lastIndex + 1) % mMaxCount;
        }

        void addTexture(const std::string& texId);
        int count = 0;

    private:
        std::string mTexId;
        Texture* mTexture = nullptr;

        friend ParticleSystem;
    };
} // namespace Plutus