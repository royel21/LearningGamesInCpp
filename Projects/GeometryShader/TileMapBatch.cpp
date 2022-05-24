#include "TileMapBatch.h"

#include <algorithm>

#include <Graphics/Shader.h>

#include <Assets/Assets.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/TileMapComponent.h>

namespace Plutus
{
    void TileMapBatch::init(TileMapComponent* tilemap)
    {
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, mVertexSize, offsetof(TileVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(2, 1, mVertexSize, offsetof(TileVert, texIndex));

        Graphic::unBind();

        loadTiles(tilemap);
    }

    void TileMapBatch::loadTiles(TileMapComponent* tilemap) {
        for (size_t i = 0; i < 16; i++) {
            texIndices[i] = i;
            textures[i] = nullptr;
        }

        mLayer = tilemap->mLayer;

        auto& mtiles = tilemap->mTiles;

        tiles.reserve(mtiles.size() * 4);

        uint32_t offset = 0;
        int totalTiles = 0;
        for (auto t : mtiles) {
            Texture* tex = tilemap->getTexture(t.texture);
            if (tex) {
                textures[t.texture] = tex;

                auto rect = tilemap->getRect(t);
                auto uv = tex->getUV(t.texcoord);

                tiles.emplace_back(rect.x, rect.y, uv.x, uv.w, t.texture);
                tiles.emplace_back(rect.x, rect.y + rect.w, uv.x, uv.y, t.texture);
                tiles.emplace_back(rect.x + rect.z, rect.y + rect.w, uv.z, uv.y, t.texture);
                tiles.emplace_back(rect.x + rect.z, rect.y, uv.z, uv.w, t.texture);

                mVerCount += 6;
                totalTiles++;
            }
        }

        mIbo.init(totalTiles);

        Graphic::uploadBufferData(mBufferId, tiles.size() * mVertexSize, tiles.data(), GL_STATIC_DRAW);
    }

    void TileMapBatch::draw(Shader* shader) {
        auto nShader = shader ? shader : mShader;
        if (nShader) {
            nShader->enable();
            nShader->setUniform4f("uColor", { 1, 1, 1, 1 });
            nShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());
            nShader->setUniform1iv("uSampler", 16, texIndices);

            Graphic::bind(mVAO);

            mIbo.bind();
            for (size_t i = 0; i < 16; i++) {
                if (textures[i]) {
                    Graphic::bindTexture(textures[i]->mTexId, i);
                }
            }

            Graphic::drawElements(mVerCount);

            mIbo.unbind();
            nShader->disable();

            Graphic::unBind();
        }
    }

    void TileMapBatch::destroy()
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mBufferId);
        mIbo.destroy();
        mVAO = -1;
        mBufferId = -1;
    }
}
