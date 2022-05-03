#pragma once

#include <vector>
#include <string>
#include <functional>

#include <Math/Vectors.h>

class b2Body;

namespace Plutus
{
    struct Particle
    {
        Vec2f position;
        Vec2f speed = { 1 };
        int size = 1;
        float lifeTime = 100;
        float elapse = 0;

        int texCoord = -1;

        b2Body* body;
        bool isBullet = false;
    };

    struct ParticleComponent
    {

        std::string mTexure;
        std::vector<Particle> mParticles;
        std::function<void(Particle&, float)> mUpdate;

        ParticleComponent();
        ParticleComponent(int maxCount, const std::string& texture = "", std::function<void(Particle&, float)> updatefunc = nullptr) { init(maxCount, texture, updatefunc); }

        void init(int maxCount, const std::string& texture = "", std::function<void(Particle&, float)> updatefunc = nullptr);

        void update(float dt);
    };
} // namespace Plutus