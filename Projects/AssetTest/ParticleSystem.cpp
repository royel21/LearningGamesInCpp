#include "ParticleSystem.h"

#include <Graphics/GLheaders.h>
#include <Graphics/GLUtils.h>
#include <Graphics/IndexBuffer.h>

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <ECS/Components.h>
#include "ParticleComponent.h"
#include <Assets/Texture.h>

#include <Graphics/Camera2D.h>


#define MAX_PARTICLES 60000

#define SHADER_PART_POS 0
#define SHADER_PART_UV 1
#define SHADER_PART_COLOR 2

namespace Plutus
{

    ParticleSystem::~ParticleSystem()
    {
        glDeleteBuffers(1, &mVBO);
        glDeleteVertexArrays(1, &mVAO);
        if (mIbo)
            delete mIbo;
    }

    void ParticleSystem::init(Project* project)
    {
        mProject = project;
        mCamera.init(project->vpWidth, project->vpHeight);

        mBatches[-1];
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);

        glEnableVertexAttribArray(SHADER_PART_POS);
        glVertexAttribPointer(SHADER_PART_POS, 2, GL_FLOAT, GL_FALSE, sizeof(RenderableParticle), (void*)NULL);
        //bind the Shader UV "Texture coordinate" to the buffer object
        glEnableVertexAttribArray(SHADER_PART_UV);
        glVertexAttribPointer(SHADER_PART_UV, 2, GL_FLOAT, GL_FALSE, sizeof(RenderableParticle), (void*)offsetof(RenderableParticle, uv));

        glEnableVertexAttribArray(SHADER_PART_COLOR);
        glVertexAttribPointer(SHADER_PART_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(RenderableParticle), (void*)offsetof(RenderableParticle, color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mIbo = new IndexBuffer(MAX_PARTICLES);

        mShader.init("assets/shaders/vertparticles.glsl", "assets/shaders/fragparticles.glsl");
    }

    void ParticleSystem::update(float dt)
    {
        prepare(dt);
        mShader.enable();
        mShader.setUniform1i("uSampler", 0);
        mShader.setUniform4f("uColor", { 1,1,1,1 });
        mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());

        mIbo->bind();
        glBindVertexArray(mVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for (auto& b : mBatches) {
            if (b.second.bufferVertices.size()) {
                mShader.setUniform1i("hasTexture", b.first > -1);

                glBufferData(GL_ARRAY_BUFFER, b.second.bufferVertices.size() * sizeof(RenderableParticle), b.second.bufferVertices.data(), GL_DYNAMIC_DRAW);

                if (b.first > 0) {
                    glBindTexture(GL_TEXTURE_2D, b.first);
                }
                glDrawElements(GL_TRIANGLES, b.second.indexCount, GL_UNSIGNED_INT, NULL);

                glBindTexture(GL_TEXTURE_2D, 0);
            }

            b.second.indexCount = 0;
            b.second.bufferVertices.clear();
            b.second.buffSize = 0;
        }
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        mIbo->unbind();
    }

    void ParticleSystem::prepare(float dt) {

        auto view = mProject->scene->getRegistry()->view<ParticleComponent>();

        Vec4f uv = { 0,0,1,1 };

        for (auto [ent, particles] : view.each())
        {
            ParticleBatch* batch = &mBatches[-1];

            auto tex = particles.mTexture;
            uint32_t size = 0;
            if (tex) {
                batch = &mBatches[tex->mTexId];
                size = particles.mTexture->uvs.size() - 1;
            }

            batch->buffSize += particles.mParticles.size();

            if (batch->buffSize > batch->bufferVertices.size())
                batch->bufferVertices.reserve(batch->buffSize);

            for (int i = particles.mParticles.size() - 1; i > 0; i--) {
                auto& p = particles.mParticles[i];
                if (p) {
                    auto color = particles.color;
                    float alpha = particles.mUpdate(p, dt);

                    if (tex) {
                        if (alpha < 1)
                            p.texCoord = int((1.0f - alpha) * size);

                        uv = particles.mTexture->getUV(p.texCoord);
                    }

                    // color.setAlpha(alpha);

                    float x = p.position.x;
                    float y = p.position.y;

                    batch->bufferVertices.emplace_back(x, y, uv.x, uv.w, color); //bottom left
                    batch->bufferVertices.emplace_back(x, y + p.size, uv.x, uv.y, color); ////Top left
                    batch->bufferVertices.emplace_back(x + p.size, y + p.size, uv.z, uv.y, color);// Top Right
                    batch->bufferVertices.emplace_back(x + p.size, y, uv.z, uv.w, color); // Bottom Right
                    batch->indexCount += 6;
                }
            }
        }
    }
}