#include "ParticleComponent.h"

#include <Assets/Assets.h>

namespace Plutus
{
    float defaultUpdate(Particle& p, float dt) {
        p.position += p.velocity * dt;
        p.elapse += dt;

        return 1.0f - p.elapse / p.lifeTime;
    }

    ParticleComponent::ParticleComponent() {
        mUpdate = defaultUpdate;
    }

    void ParticleComponent::init(int maxCount, const std::string& texId, std::function<float(Particle&, float)> updatefunc)
    {
        mTexId = texId;
        mMaxCount = maxCount;
        mTexture = AssetManager::get()->getAsset<Texture>(texId);
        mUpdate = updatefunc ? updatefunc : defaultUpdate;
        mParticles.resize(maxCount);
    }

    void ParticleComponent::addTexture(const std::string& texId) {
        mTexId = texId;
        mTexture = AssetManager::get()->getAsset<Texture>(texId);
    }
}