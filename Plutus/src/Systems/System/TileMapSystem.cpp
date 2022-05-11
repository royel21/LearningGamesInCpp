#include "TileMapSystem.h"
#include <Core/Project.h>

#include <ECS/Scene.h>
#include <Assets/Assets.h>
#include <Graphics/Graphic.h>
#include <ECS/Components/TileMapComponent.h>

#include <Graphics/Camera2D.h>

#include <algorithm>

#include "MapShader.h"

namespace Plutus
{
    struct TileVert {
        float x;
        float y;
        float index;
        TileVert(float _x, float _y, float i) : x(_x), y(_y), index(i) {}
    };

    struct tempMap {
        std::string name;
        TileMapComponent* map;

        bool operator < (const tempMap& ref) const {
            return map->mLayer < ref.map->mLayer;
        }
    };

    void TileMapSystem::init(Project* project)
    {
        mProject = project;
        mShader.init(MapShader::vertexShader, MapShader::fragShader, MapShader::geoShader);

        auto mapView = mProject->scene->getRegistry()->view<TileMapComponent>();
        std::vector<tempMap> maps;

        for (auto [e, tmap] : mapView.each()) {
            auto ent = Entity{ e, mProject->scene.get() };
            maps.push_back({ ent.getName(), &tmap });
        }

        std::sort(maps.begin(), maps.end());

        for (auto& m : maps) {
            addMap(m.name, m.map);
        }
    }

    void TileMapSystem::destroy()
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

    void TileMapSystem::addMap(const std::string& id, TileMapComponent* tmap)
    {
        tileMaps.emplace_back();
        auto& currentMap = tileMaps.back();

        tileWidth = tmap->mTileWidth;
        tileHeight = tmap->mTileHeight;

        std::unordered_map<std::string, std::vector<TileVert>> tilesMap;
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
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileVert), (void*)NULL);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(TileVert), (void*)offsetof(TileVert, index));

            glBufferData(GL_ARRAY_BUFFER, tilemp.second.size() * sizeof(TileVert), tilemp.second.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void TileMapSystem::update(float dt)
    {
        mShader.enable();
        for (const auto& batchs : tileMaps) {
            for (const auto& batch : batchs) {
                glBindVertexArray(batch.second.vertId);
                glBindBuffer(GL_ARRAY_BUFFER, batch.second.buffId);

                auto tex = AssetManager::get()->getAsset<Texture>(batch.first);
                glBindTexture(GL_TEXTURE_2D, tex->mTexId);

                mShader.setUniform1i("hasTexture", 1);
                mShader.setUniform1f("tw", (float)tileWidth);
                mShader.setUniform1f("th", (float)tileHeight);

                mShader.setUniform1fv("uTexData", 5, &tex->mTileSet.columns);

                mShader.setUniform4f("uColor", { 1.0f, 1.0f,1.0f, 1.0f });
                mShader.setUniformMat4("uCamera", mCamera->getCameraMatrix());

                glDrawArrays(GL_POINTS, 0, batch.second.vertCount);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }
        mShader.disable();
    }
}

