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
        //Size of Vertex Struct in bytes
        auto vsize = sizeof(TileVert);
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, vsize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, vsize, (void*)offsetof(TileVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(2, 1, vsize, (void*)offsetof(TileVert, texIndex));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

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

        Graphic::uploadBufferData(mBufferId, tiles.size() * sizeof(TileVert), tiles.data(), GL_STATIC_DRAW);
    }

    void TileMapBatch::draw() {
        mShader->enable();
        mShader->setUniform4f("uColor", { 1, 1, 1, 1 });
        mShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());
        mShader->setUniform1iv("uSampler", 16, texIndices);

        glBindVertexArray(mVAO);

        mIbo.bind();
        for (size_t i = 0; i < 16; i++) {
            if (textures[i]) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i]->mTexId);
            }
        }

        glDrawElements(GL_TRIANGLES, mVerCount, GL_UNSIGNED_INT, NULL);

        mIbo.unbind();
        mShader->disable();

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
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

