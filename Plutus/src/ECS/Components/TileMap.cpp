#include "TileMap.h"
#include <algorithm>
#include <Assets/AssetManager.h>

namespace Plutus
{

    // TileMap::TileMap(const TileMap& tilemap){
    //     mTileWidth = tilemap.mTileWidth;
    //     mTileHeight = tilemap.mTileHeight;
    //     mLayer = tilemap.mLayer;
    //     mTiles = tilemap.mTiles;
    //     mTextures = tilemap.mTextures;
    // }

    void TileMap::addTexture(const std::string& texture)
    {
        mTextures[(int)mTextures.size()] = AssetManager::get()->mTextures.getTexture(texture);
    }

    void TileMap::removeTexture(int index)
    {
        if (mTiles.size()) {
            for (int i = (int)mTiles.size() - 1; i > -1; i--) {
                if (mTiles[i].texture == index) {
                    mTiles.erase(mTiles.begin() + i);
                }
            }
        }

        auto it = mTextures.find(index);
        if (it != mTextures.end()) {
            mTextures.erase(it);
        }
    }

    void TileMap::addTile(const Tile& tile)
    {
        mTiles.push_back(tile);
    }

    Tile* TileMap::getTile(const glm::ivec2& mCoords)
    {
        int index = getIndex(mCoords);
        return index > -1 ? &mTiles[index] : nullptr;
    }

    int TileMap::getIndex(const glm::ivec2& pos)
    {
        auto it = std::find_if(mTiles.begin(), mTiles.end(), [=](const Tile& ntile) -> bool
            { return ntile.x >= pos.x && ntile.x <= pos.x + mTileWidth - 1 && ntile.y >= pos.y && ntile.y <= pos.y + mTileHeight - 1; });

        return it != mTiles.end() ? int(it - mTiles.begin()) : -1;
    }

    bool TileMap::removeTile(const glm::ivec2& mCoords)
    {
        int index = getIndex(mCoords);
        if (index > -1)
        {
            mTiles.erase(mTiles.begin() + index);
        }
        return index > -1;
    }

} // namespace Plutus
