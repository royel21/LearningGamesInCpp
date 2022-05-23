#include "SpriteBatch.h"

#include <algorithm>

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>
#include <Graphics/Renderables.h>
#include <Graphics/Camera2D.h>

#include "GLSLBatch.h"

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2

namespace Plutus {

    void SpriteBatch::init(uint32_t MAX_SPRITE) {
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        auto vsize = sizeof(SpriteVert);
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(SHADER_VERTEX_INDEX, 2, vsize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(SHADER_UV_INDEX, 2, vsize, (void*)offsetof(SpriteVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(SHADER_COLOR_INDEX, 4, vsize, (void*)offsetof(SpriteVert, color), GL_UNSIGNED_BYTE, GL_TRUE);

        mIbo.init(MAX_SPRITE);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SpriteBatch::addSprite(Renderable* renderable) {
        createBatch(renderable->texture);

        auto uv = renderable->uv;
        auto& rect = renderable->trans;

        if (renderable->flipX)
            std::swap(uv.x, uv.z);

        if (renderable->flipY)
            std::swap(uv.y, uv.w);

        float w = rect.x + rect.z;
        float h = rect.y + rect.w;

        mbottomLeft = { rect.x, rect.y };
        mtopLeft = { rect.x, h };
        mtopRight = { w, h };
        mBottomRight = { w, rect.y };

        if (renderable->r)
        {
            Vec2f center(w * 0.5f, h * 0.5f);

            rotate(mbottomLeft, center, renderable->r);
            rotate(mtopLeft, center, renderable->r);
            rotate(mtopRight, center, renderable->r);
            rotate(mBottomRight, center, renderable->r);
        }

        sprites.emplace_back(mbottomLeft.x, mbottomLeft.y, uv.x, uv.w, renderable->color);
        sprites.emplace_back(mtopLeft.x, mtopLeft.y, uv.x, uv.y, renderable->color);
        sprites.emplace_back(mtopRight.x, mtopRight.y, uv.z, uv.y, renderable->color);
        sprites.emplace_back(mBottomRight.x, mBottomRight.y, uv.z, uv.w, renderable->color);
    }

    void SpriteBatch::createBatch(Texture* tex) {
        BatchTex* batch = nullptr;

        if (mBatches.size() > 0 && mBatches.back().texture == tex)
        {
            batch = &mBatches.back();
        }
        else
        {
            mBatches.emplace_back();
            batch = &mBatches.back();
            batch->texture = tex;
            batch->iboOffset = offset;
        }

        offset += 6;
        batch->vertCount += 6;
    }

    void SpriteBatch::draw() {
        if (mShader) {
            mShader->enable();
            mShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());

            Graphic::enableBlend();

            glBindVertexArray(mVAO);


            Graphic::uploadBufferData(mBufferId, sprites.size() * sizeof(SpriteVert), sprites.data(), GL_DYNAMIC_DRAW);

            mIbo.bind();

            for (auto& batch : mBatches)
            {
                if (batch.texture) {
                    mShader->setUniform1i("uHasTex", 1);
                    mShader->setUniform1i("uSampler", batch.texture->mTexureUnit);

                    glBindTexture(GL_TEXTURE_2D, batch.texture->mTexId);
                    glActiveTexture(GL_TEXTURE0 + batch.texture->mTexureUnit);
                }
                else {
                    mShader->setUniform1i("uHasTex", 0);
                }

                glDrawElements(GL_TRIANGLES, batch.vertCount, GL_UNSIGNED_INT, (void*)(batch.iboOffset * sizeof(GLuint)));

            }

            mIbo.unbind();
            mShader->disable();

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

        }
        sprites.clear();
        mBatches.clear();
        offset = 0;
    }

    void SpriteBatch::destroy() {
        glDeleteBuffers(1, &mBufferId);
        glDeleteVertexArrays(1, &mVAO);
        mIbo.destroy();
    }
} // namespace Plutus