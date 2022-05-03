#include "ParticleSystem.h"

#include <Graphics/GraphicsUtil.h>
#include <Graphics/IndexBuffer.h>

#include <ECS/Scene.h>
#include <Core/Project.h>
#include <ECS/Components.h>
#include "ParticleComponent.h"
#include <Assets/Texture.h>


#define MAX_PARTICLES 60000

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
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*)NULL);
        //bind the Shader UV "Texture coordinate" to the buffer object
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*)offsetof(RenderableParticle, uv));


        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mIbo = new IndexBuffer(MAX_PARTICLES);
    }

    void ParticleSystem::update(float dt)
    {
        // mShader.enable();
        // glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        // glBufferData(GL_ARRAY_BUFFER, mVertexCount * sizeof(RenderableParticle), bufferVertices.data(), GL_DYNAMIC_DRAW);
        // glBindVertexArray(mVAO);




        // glBindVertexArray(0);
        // glBindTexture(GL_TEXTURE_2D, 0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // mShader.disable();
    }

    void ParticleSystem::prepare(float dt) {
        mProject->scene->getRegistry()->sort<ParticleComponent>([](const auto& lhs, const auto& rhs) {
            return lhs.mTexture < rhs.mTexture;
            });

        auto view = mProject->scene->getRegistry()->view<TransformComponent, ParticleComponent>();
        uint32_t size = 0;

        for (auto [ent, trans, particles] : view.each())
        {
            Batch* batch = &mBatches[-1];
            if (particles.mTexture) {
                batch = &mBatches[particles.mTexture->mTexId];
            }


            size = particles.mParticles.size();

            if (batch->bufferVertices.size() < size) {
                batch->bufferVertices.resize(size);
            }

            // size += particles.mParticles.size();
            // if()

            // Batch* batch = &mBatches[-1];
            // if (particles.mTexture) {
            //     batch = &mBatches[particles.mTexture->mTexId];
            // }

            // batch->offset = mIndexCount;
            // batch->numVerts += 6;

            // if (bufferVertices.size() < size) bufferVertices.resize(size);

            for (auto& p : particles.mParticles) {
                if (p) {
                    particles.mUpdate(p, dt);
                    Vec4f uv;
                    if (particles.mTexture) {
                        uv = particles.mTexture->getUV(p.texCoord);
                    }

                    float x = particles.offset.x + p.position.x;
                    float y = particles.offset.y + p.position.y;

                    // bufferVertices[mVertexCount++] = { {x, y}, {uv.x, uv.w} };
                    // bufferVertices[mVertexCount++] = { {x, x + p.size}, {uv.x, uv.y} };
                    // bufferVertices[mVertexCount++] = { {x + p.size}, {y + p.size} };
                    // bufferVertices[mVertexCount++] = { {x + p.size, y}, {uv.z, uv.w} };
                    mIndexCount += 6;
                }
            }
        }
    }
}