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
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
    }

    void ParticleSystem::init(Project* project)
    {
        mProject = project;

        mShader.init(ParticleShader::vertexShader, ParticleShader::fragShader, ParticleShader::geoShader);

        mBatches[-1];
        mVAO = Graphic::createVertexArray();
        mVBO = Graphic::createBufferArray();

        auto vsize = sizeof(RenderableParticle);
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(SHADER_P_POS, 2, vsize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(SHADER_P_SIZE, 1, vsize, offsetof(RenderableParticle, size));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(SHADER_P_COLOR, 4, vsize, offsetof(RenderableParticle, color), GL_UNSIGNED_BYTE, GL_TRUE);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        if (mBlend) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else {
            glDisable(GL_BLEND);
        }

        for (auto& b : mBatches) {
            if (b.second.bufferVertices.size()) {
                if (b.second.tex != nullptr) {
                    glActiveTexture(GL_TEXTURE0 + b.second.tex->mTexureUnit);
                    glBindTexture(GL_TEXTURE_2D, b.second.tex->mTexId);
                    mShader.setUniform1i("uSampler", b.second.tex->mTexureUnit);
                    mShader.setUniform1i("uhasTexture", 1);
                    mShader.setUniform1fv("uTexData", 5, &b.second.tex->mTileSet.columns);
                }
                glBufferData(GL_ARRAY_BUFFER, b.second.bufferVertices.size() * sizeof(RenderableParticle), b.second.bufferVertices.data(), GL_DYNAMIC_DRAW);

                glDrawArrays(GL_POINTS, 0, b.second.bufferVertices.size());

                glBindTexture(GL_TEXTURE_2D, 0);
                b.second.bufferVertices.clear();
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}