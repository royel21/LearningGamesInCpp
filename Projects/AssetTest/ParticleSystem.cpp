#include "ParticleSystem.h"


#include <ECS/Scene.h>
#include <Core/Project.h>
#include <ECS/Components.h>
#include "ParticleComponent.h"
#include <Assets/Texture.h>

#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <Graphics/IndexBuffer.h>

#include "ParticleShader.h"

#include <Log/Logger.h>
#include <Time/Timer.h>

#define SHADER_P_POS 0
#define SHADER_P_SIZE 1
#define SHADER_P_COLOR 2

namespace Plutus
{

    ParticleSystem::~ParticleSystem()
    {
        Graphic::destroy(&mVAO, &mVBO);
    }

    void ParticleSystem::init(Project* project)
    {
        mProject = project;

        mShader.init(ParticleShader::vertexShader, ParticleShader::fragShader, ParticleShader::geoShader);

        mBatches[-1];
        mVAO = Graphic::createVertexArray();
        mVBO = Graphic::createBufferArray();

        mVerByteSize = sizeof(RenderableParticle);
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(SHADER_P_POS, 2, mVerByteSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(SHADER_P_SIZE, 1, mVerByteSize, offsetof(RenderableParticle, size));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(SHADER_P_COLOR, 4, mVerByteSize, offsetof(RenderableParticle, color), GL_UNSIGNED_BYTE, GL_TRUE);

        Graphic::unBind();
    }

    bool ParticleSystem::prepare(float dt) {

        auto view = mProject->scene->getRegistry()->view<ParticleComponent>();
        bool hasParticle = false;

        for (auto [ent, particles] : view.each())
        {
            auto batch = &mBatches[-1];

            if (particles.mTexture) {
                batch = &mBatches[particles.mTexture->mTexId];
                batch->tex = particles.mTexture;
            }

            batch->bufferVertices.reserve(particles.mParticles.size());

            for (auto& p : particles.mParticles) {
                if (p) {
                    hasParticle = true;
                    auto color = particles.color;
                    float alpha = particles.mUpdate(p, dt);

                    if (alpha == 0) particles.count--;

                    color.a = uint8_t((1.0f - alpha) * 255);

                    batch->bufferVertices.emplace_back(p.position, (float)p.size, color);
                }
            }
        }
        return hasParticle;
    }

    void ParticleSystem::update(float dt)
    {
        if (!prepare(dt)) return;

        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera->getCameraMatrix());

        Graphic::bind(mVAO, mVBO);

        if (mBlend) {
            Graphic::enableBlend();
        }
        else {
            Graphic::disableBlend();
        }

        for (auto& b : mBatches) {
            auto& batch = b.second;
            auto size = batch.bufferVertices.size();

            if (size) {

                if (batch.tex != nullptr) {
                    Graphic::bindTexture(batch.tex->mTexId, batch.tex->mTexureUnit);
                    mShader.setUniform1i("uSampler", batch.tex->mTexureUnit);
                    mShader.setUniform1i("uhasTexture", 1);
                    mShader.setUniform1fv("uTexData", 5, &batch.tex->mTileSet.columns);
                }
                Graphic::uploadBufferData(mVBO, size * mVerByteSize, batch.bufferVertices.data());

                glDrawArrays(GL_POINTS, 0, size);

                Graphic::unBindTexture();
                batch.bufferVertices.clear();
            }
        }

        Graphic::unBind();
    }
}