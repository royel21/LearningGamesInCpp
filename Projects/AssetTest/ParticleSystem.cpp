#include "ParticleSystem.h"

#include <Graphics/GLheaders.h>
#include <Graphics/GLUtils.h>
#include <Graphics/IndexBuffer.h>

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <ECS/Components.h>
#include "ParticleComponent.h"
#include <Assets/Texture.h>

#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>

#include "ParticleShader.h"


#define MAX_PARTICLES 60000

#define SHADER_P_POS 0
#define SHADER_P_SIZE 1
#define SHADER_P_TEXTCOORD 2
#define SHADER_P_COLOR 3

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
        mCamera.init(project->vpWidth, project->vpHeight);

        mShader.init(ParticleShader::vertexShader, ParticleShader::fragShader, ParticleShader::geoShader);

        mBatches[-1];
        mVAO = Graphic::createVertexArray();
        mVBO = Graphic::createArrayBuffer();

        glEnableVertexAttribArray(SHADER_P_POS);
        glVertexAttribPointer(SHADER_P_POS, 2, GL_FLOAT, GL_FALSE, sizeof(RenderableParticle), (void*)NULL);

        glEnableVertexAttribArray(SHADER_P_SIZE);
        glVertexAttribPointer(SHADER_P_SIZE, 1, GL_FLOAT, GL_FALSE, sizeof(RenderableParticle), (void*)offsetof(RenderableParticle, size));
        //bind the Shader UV "Texture coordinate" to the buffer object
        glEnableVertexAttribArray(SHADER_P_TEXTCOORD);
        glVertexAttribPointer(SHADER_P_TEXTCOORD, 1, GL_FLOAT, GL_FALSE, sizeof(RenderableParticle), (void*)offsetof(RenderableParticle, texcoord));

        glEnableVertexAttribArray(SHADER_P_COLOR);
        glVertexAttribPointer(SHADER_P_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(RenderableParticle), (void*)offsetof(RenderableParticle, color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ParticleSystem::update(float dt)
    {
        if (!prepare(dt)) return;

        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());

        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    bool ParticleSystem::prepare(float dt) {

        auto view = mProject->scene->getRegistry()->view<ParticleComponent>();
        bool hasParticle = false;

        for (auto [ent, particles] : view.each())
        {
            auto batch = &mBatches[-1];

            uint32_t size = 0;

            if (particles.mTexture) {
                batch = &mBatches[particles.mTexture->mTexId];
                batch->tex = particles.mTexture;
                size = particles.mTexture->uvs.size() - 1;
            }

            batch->bufferVertices.reserve(particles.mParticles.size());

            // for (int i = particles.mParticles.size() - 1; i > 0; i--) {
                // auto& p = particles.mParticles[i];
            for (auto& p : particles.mParticles) {
                if (p) {
                    hasParticle = true;
                    auto color = particles.color;
                    float alpha = particles.mUpdate(p, dt);

                    if (batch->tex && alpha < 1) {
                        p.texCoord = int((1.0f - alpha) * size);
                    }

                    color.setAlpha(alpha);

                    batch->bufferVertices.emplace_back(p.position, (float)p.size, (float)p.texCoord, color);
                }
            }
        }
        return hasParticle;
    }
}