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
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(SHADER_VERTEX_INDEX, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(SHADER_UV_INDEX, 2, mVertexSize, offsetof(SpriteVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(SHADER_COLOR_INDEX, 4, mVertexSize, offsetof(SpriteVert, color), GL_UNSIGNED_BYTE, GL_TRUE);

        mIbo.init(MAX_SPRITE);

        Graphic::unBind();
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

            PMath::rotate(mbottomLeft, center, renderable->r);
            PMath::rotate(mtopLeft, center, renderable->r);
            PMath::rotate(mtopRight, center, renderable->r);
            PMath::rotate(mBottomRight, center, renderable->r);
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

    void SpriteBatch::draw(Shader* shader) {
        auto nShader = shader ? shader : mShader;

        if (nShader) {
            nShader->enable();
            nShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());


            Graphic::bind(mVAO);
            Graphic::uploadBufferData(mBufferId, sprites.size() * mVertexSize, sprites.data());

            mIbo.bind();

            for (auto& batch : mBatches)
            {
                if (batch.texture) {
                    nShader->setUniform1i("uHasTex", 1);
                    nShader->setUniform1i("uSampler", batch.texture->mTexureUnit);

                    Graphic::bindTexture(batch.texture->mTexId, batch.texture->mTexureUnit);
                }
                else {
                    nShader->setUniform1i("uHasTex", 0);
                }
                Graphic::drawElements(batch.vertCount, batch.iboOffset);
            }

            nShader->disable();

            Graphic::unBind();

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