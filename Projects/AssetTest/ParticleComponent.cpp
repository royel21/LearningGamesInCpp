#include "ParticleComponent.h"

#include <Assets/Assets.h>

namespace Plutus
{
    void defaultUpdate(Particle& p, float dt) {
        p.position += p.velocity * dt;
        p.lifeTime -= dt;
    }

    ParticleComponent::ParticleComponent() {
        mUpdate = defaultUpdate;
    }

    void ParticleComponent::init(int maxCount, const std::string& texId, std::function<void(Particle&, float)> updatefunc)
    {
        mTexId = texId;
        mMaxCount = maxCount;
        mTexture = AssetManager::get()->getAsset<Texture>(texId);
        mUpdate = updatefunc ? updatefunc : defaultUpdate;
        mParticles.resize(maxCount);
    }
}