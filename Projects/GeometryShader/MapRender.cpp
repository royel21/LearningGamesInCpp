#include "MapRender.h"

#include <ECS/Components/TileMapComponent.h>
#include <cstdio>

#include <Graphics/Graphic.h>
#include <Assets/Assets.h>

#include "Camera.h"

namespace Plutus
{
    MapRender::~MapRender()
    {
        for (auto& batches : tileMaps) {
            for (auto& tm : batches) {
                glDeleteVertexArrays(1, &tm.second.vertId);
                glDeleteBuffers(1, &tm.second.buffId);
            }
        }

        tileMaps.clear();
        mShader.destroy();
    }

    struct Tile1 {
        float x;
        float y;
        float index;
        Tile1(float _x, float _y, float i) : x(_x), y(_y), index(i) {}
    };

    void MapRender::init(Camera* camera)
    {
        if (!mShader.init("Projects/GeometryShader/ver.glsl", "Projects/GeometryShader/frag.glsl", "Projects/GeometryShader/geoquad.glsl")) {
            return;
        }

        mCamera = camera;
    }

    void MapRender::addMap(const std::string& id, TileMapComponent* tmap)
    {
        tileMaps.emplace_back();
        auto& currentMap = tileMaps.back();

        tileWidth = tmap->mTileWidth;
        tileHeight = tmap->mTileHeight;

        std::unordered_map<std::string, std::vector<Tile1>> tilesMap;
        std::string tex = "";
        for (auto& tile : tmap->mTiles) {
            if (tmap->mTextures[tile.texture] != tex) {
                tex = tmap->mTextures[tile.texture];
                tilesMap[tex].reserve(tmap->mTiles.size());
            }
            tilesMap[tex].emplace_back((float)tile.x, (float)tile.y, (float)tile.texcoord);
        }

        for (auto tilemp : tilesMap) {
            auto& batch = currentMap[tilemp.first];

            batch.vertId = Graphic::createVertexArray();
            batch.buffId = Graphic::createVertexBuffer();
            batch.vertCount = tilemp.second.size();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Tile1), (void*)NULL);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Tile1), (void*)offsetof(Tile1, index));

            glBufferData(GL_ARRAY_BUFFER, tilemp.second.size() * sizeof(Tile1), tilemp.second.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void MapRender::draw()
    {
        std::string texId = "";
        int i = 0;
        mShader.enable();
        for (const auto& batchs : tileMaps) {
            if (i > 0)
                for (const auto& batch : batchs) {
                    glBindVertexArray(batch.second.vertId);
                    glBindBuffer(GL_ARRAY_BUFFER, batch.second.buffId);

                    auto tex = AssetManager::get()->getAsset<Texture>(batch.first);
                    glBindTexture(GL_TEXTURE_2D, tex->mTexId);
                    mShader.setUniform1fv("uTexData", 5, &tex->mTileSet.columns);

                    glActiveTexture(GL_TEXTURE0 + tex->mTexureUnit);

                    mShader.setUniform1i("hasTexture", 1);
                    mShader.setUniform1i("mySampler", 0);
                    mShader.setUniform1f("tw", (float)tileWidth);
                    mShader.setUniform1f("th", (float)tileHeight);

                    mShader.setUniform4f("uColor", { 1.0f, 1.0f,1.0f, 1.0f });
                    mShader.setUniformMat4("uCamera", mCamera->getCamera());

                    glDrawArrays(GL_POINTS, 0, batch.second.vertCount);

                    // glBindTexture(GL_TEXTURE_2D, 0);
                    // glBindBuffer(GL_ARRAY_BUFFER, 0);
                    // glBindVertexArray(0);
                    break;
                }
            i++;
        }
        mShader.disable();
    }
}