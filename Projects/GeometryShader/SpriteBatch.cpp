#include "SpriteBatch.h"

#include <algorithm>

#include <Math/PMath.h>
#include <Graphics/Shader.h>
#include <Graphics/Graphic.h>

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2
#define MAX_SPRITE 60000

namespace Plutus {

    void SpriteBatch::init() {
        mShader = new Shader();
        mShader->init();
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        //bind the Shader position to the buffer object
        glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
        glVertexAttribPointer(SHADER_VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVert), (void*)NULL);
        //bind the Shader UV "Texture coordinate" to the buffer object
        glEnableVertexAttribArray(SHADER_UV_INDEX);
        glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVert), (void*)offsetof(SpriteVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        glEnableVertexAttribArray(SHADER_COLOR_INDEX);
        glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SpriteVert), (void*)offsetof(SpriteVert, color));

        mIbo.init(MAX_SPRITE);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void SpriteBatch::addSprite(uint32_t texId, const Vec4f& rect, Vec4f& uv, ColorRGBA8 c, bool flipX, bool flipY, float r) {
        if (flipX)
            std::swap(uv.x, uv.z);

        if (flipY)
            std::swap(uv.y, uv.w);

        mbottomLeft = { rect.x, rect.y };
        mtopLeft = { rect.x, rect.y + rect.w };
        mtopRight = { rect.x + rect.z, rect.y + rect.w };
        mBottomRight = { rect.x + rect.z, rect.y };

        if (r)
        {
            Vec2f center(rect.x + rect.z * 0.5f, rect.y + rect.w * 0.5f);
            rotate(mbottomLeft, center, r);
            rotate(mtopLeft, center, r);
            rotate(mtopRight, center, r);
            rotate(mBottomRight, center, r);
        }

        sprites.emplace_back(mbottomLeft.x, mbottomLeft.y, uv.x, uv.w, c);
        sprites.emplace_back(mtopLeft.x, mtopLeft.y, uv.x, uv.y, c);
        sprites.emplace_back(mtopRight.x, mtopRight.y, uv.z, uv.y, c);
        sprites.emplace_back(mBottomRight.x, mBottomRight.y, uv.z, uv.w, c);
    }

    void SpriteBatch::draw() {

    }

    void SpriteBatch::destroy() {
        mShader->destroy();
        glDeleteBuffers(1, &mBufferId);
        glDeleteVertexArrays(1, &mVAO);
        mIbo.destroy();
        delete mShader;
    }
} // namespace Plutus