#include "ParticleComponent.h"

namespace Plutus
{
    void defaultUpdate(Particle& p, float dt) {
        p.position += p.speed * dt;
    }

    ParticleComponent::ParticleComponent() {
        mUpdate = defaultUpdate;
    }

    void ParticleComponent::init(int maxCount, const std::string& texture, std::function<void(Particle&, float)> updatefunc)
    {
        mTexure = texture;
        mUpdate = updatefunc ? updatefunc : defaultUpdate;
        mParticles.reserve(maxCount);
    }

    void ParticleComponent::update(float dt) {

        for (auto& p : mParticles) {
            if (p.lifeTime < 0) {
                mUpdate(p, dt);
            }
        }
    }
}