#include "ParticleSystem.h"

#include <Graphics/GLheaders.h>
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
            Batch* batch = &mBatches[-1];

            if (particles.mTexture) {
                batch = &mBatches[particles.mTexture->mTexId];
            }

            batch->buffSize += particles.mParticles.size();

            if (batch->buffSize > batch->bufferVertices.size())
                batch->bufferVertices.reserve(batch->buffSize);

            for (auto& p : particles.mParticles) {
                if (p) {
                    particles.mUpdate(p, dt);

                    if (particles.mTexture) {
                        uv = particles.mTexture->getUV(p.texCoord);
                    }

                    float x = p.position.x;
                    float y = p.position.y;

                    batch->bufferVertices.emplace_back(x, y, uv.x, uv.w); //bottom left
                    batch->bufferVertices.emplace_back(x, y + p.size, uv.x, uv.y); ////Top left
                    batch->bufferVertices.emplace_back(x + p.size, y + p.size, uv.z, uv.y);// Top Right
                    batch->bufferVertices.emplace_back(x + p.size, y, uv.z, uv.w); // Bottom Right
                    batch->indexCount += 6;
                }
            }
        }
    }
}