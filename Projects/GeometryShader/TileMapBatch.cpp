#include "TileMapBatch.h"

#include <algorithm>

#include <Graphics/Shader.h>

#include <Assets/Assets.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/TileMapComponent.h>

namespace Plutus
{
    struct TileVert {
        float x;
        float y;
        float uvx;
        float uvy;
        TileVert(float _x, float _y, float _ux, float _uy) : x(_x), y(_y), uvx(_ux), uvy(_uy) {}
    };


    void TileMapBatch::init(TileMapComponent* tilemap)
    {
        mLayer = tilemap->mLayer;
        std::vector<TileVert> tiles;

        auto& mtiles = tilemap->mTiles;

        tiles.reserve(mtiles.size() * 4);

        std::sort(mtiles.begin(), mtiles.end());

        uint32_t offset = 0;

        Texture* tex = nullptr;
        BatchTex* batch = nullptr;
        for (auto t : mtiles) {
            auto tTex = tilemap->getTexture(t.texture);
            if (tTex) {
                if (tTex != tex) {
                    tex = tTex;
                    batch = &mBatches[tex->mTexId];
                    batch->texture = tTex;
                    batch->iboOffset = offset;
                }

                auto rect = tilemap->getRect(t);
                auto uv = tex->getUV(t.texcoord);

                tiles.emplace_back(rect.x, rect.y, uv.x, uv.w);
                tiles.emplace_back(rect.x, rect.y + rect.w, uv.x, uv.y);
                tiles.emplace_back(rect.x + rect.z, rect.y + rect.w, uv.z, uv.y);
                tiles.emplace_back(rect.x + rect.z, rect.y, uv.z, uv.w);

                batch->vertCount += 6;
                offset += 6;
            }
        }

        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileVert), (void*)NULL);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TileVert), (void*)offsetof(TileVert, uvx));

        mIbo.init(mtiles.size());

        glBufferData(GL_ARRAY_BUFFER, tiles.size() * sizeof(TileVert), tiles.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void TileMapBatch::draw() {
        mShader->enable();
        mShader->setUniform4f("uColor", { 1, 1, 1, 1 });
        mShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());

        glBindVertexArray(mVAO);
        mIbo.bind();
        for (auto& batch : mBatches) {
            mShader->setUniform1i("uSampler", batch.second.texture->mTexureUnit);
            glActiveTexture(GL_TEXTURE0 + batch.second.texture->mTexureUnit);
            glBindTexture(GL_TEXTURE_2D, batch.second.texture->mTexId);
            glDrawElements(GL_TRIANGLES, batch.second.vertCount, GL_UNSIGNED_INT, (void*)(batch.second.iboOffset * sizeof(GLuint)));
        }

        mIbo.unbind();
        glBindVertexArray(0);
        mShader->disable();
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

