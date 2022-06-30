#include "SpriteBatch.h"

#include <algorithm>

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <Graphics/Renderables.h>

#include <Assets/Assets.h>

#include "GLSLBatch.h"

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2

namespace Plutus {

    void SpriteBatch::init(uint32_t MAX_SPRITE) {
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        mIboId = Graphic::createElementBuffer();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(SHADER_VERTEX_INDEX, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(SHADER_UV_INDEX, 2, mVertexSize, offsetof(SpriteVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setFAttribute(SHADER_COLOR_INDEX, 4, mVertexSize, offsetof(SpriteVert, color), GL_UNSIGNED_BYTE, GL_TRUE);

        Graphic::uploadIndices(mIboId, MAX_SPRITE);

        Graphic::unBind();
    }

    void SpriteBatch::addSprite(Renderable* renderable) {
        createBatch(renderable->texture->mTexId, renderable->texture->mTexureUnit);

        auto uv = renderable->uv;

        if (renderable->flipX)
            std::swap(uv.x, uv.z);

        if (renderable->flipY)
            std::swap(uv.y, uv.w);

        auto vertices = renderable->trans.getvertices();

        if (renderable->r)
        {
            Vec2f center = vertices[2] * 0.5f;
            PMath::rotate(vertices[0], center, renderable->r); // Bottom Left
            PMath::rotate(vertices[1], center, renderable->r); // Top Left
            PMath::rotate(vertices[2], center, renderable->r); // Top Right
            PMath::rotate(vertices[3], center, renderable->r); // Bottom Left
        }

        sprites.emplace_back(vertices[0], uv.x, uv.w, renderable->color); // Bottom Left
        sprites.emplace_back(vertices[1], uv.x, uv.y, renderable->color); // Top Left
        sprites.emplace_back(vertices[2], uv.z, uv.y, renderable->color); // Top Right
        sprites.emplace_back(vertices[3], uv.z, uv.w, renderable->color); // Bottom Left
    }


    void SpriteBatch::addText(const std::string& fontId, float x, float y, const std::string& text, ColorRGBA8 color, float scale) {
        auto font = AssetManager::get()->getAsset<Font>(fontId);
        if (font != nullptr) {

            sprites.reserve(sprites.size() + (text.size() * 4));
            scale *= 1.0f / mCamera->getScale();
            for (auto i : text) {
                createBatch(font->mTexId, 0);
                auto& ch = font->ch[i];
                GLfloat xpos = x + ch.bl * scale;
                GLfloat ypos = y - (ch.bh - ch.bt) * scale; // shift the letter down for Top-Left origin camera

                // Advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += ch.ax * scale;

                auto vertices = Vec4f(xpos, ypos, ch.bw * scale, ch.bh * scale).getvertices();

                sprites.emplace_back(vertices[0], ch.uv.x, ch.uv.w, color); // Bottom Left
                sprites.emplace_back(vertices[1], ch.uv.x, ch.uv.y, color); // Top Left
                sprites.emplace_back(vertices[2], ch.uv.z, ch.uv.y, color); // Top Right
                sprites.emplace_back(vertices[3], ch.uv.z, ch.uv.w, color); // Bottom Left
            }
        }
    }

    void SpriteBatch::createBatch(uint32_t texId, uint32_t texUnit) {
        BatchTex* batch = nullptr;

        if (mBatches.size() > 0 && mBatches.back().texId == texId)
        {
            batch = &mBatches.back();
        }
        else
        {
            mBatches.emplace_back();
            batch = &mBatches.back();
            batch->texId = texId;
            batch->texUnit = texUnit;
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

            Graphic::enableBlend();

            Graphic::bind(mVAO, mIboId);

            Graphic::uploadBufferData(mBufferId, sprites.size() * mVertexSize, sprites.data());

            for (auto& batch : mBatches)
            {
                if (batch.texId) {
                    mShader->setUniform1i("uHasTex", 1);
                    mShader->setUniform1i("uSampler", batch.texUnit);

                    Graphic::bindTexture(batch.texId, batch.texUnit);
                }
                else {
                    mShader->setUniform1i("uHasTex", 0);
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
        Graphic::destroy(&mVAO, &mBufferId, &mIboId);
    }
} // namespace Plutus