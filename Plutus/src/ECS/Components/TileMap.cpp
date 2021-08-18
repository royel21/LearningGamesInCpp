#include "TileMap.h"
#include <algorithm>

namespace Plutus
{

    void TileMap::addTexture(const std::string &texture)
    {
        mTextures.push_back(Textures::get()->getTexture(texture));
    }

    void TileMap::removeTexture(const std::string &texture)
    {
        auto it = std::find_if(mTextures.begin(), mTextures.end(), [texture](const TileSet *tileset) -> bool
                               { return tileset->name.compare(texture) == 0; });
        mTextures.erase(it);
    }

    void TileMap::addTile(const Tile &tile)
    {
        mTiles.push_back(tile);
    }

    Tile *TileMap::getTile(const glm::ivec2 &mCoords)
    {
        int index = getIndex(mCoords);
        return index > -1 ? &mTiles[index] : nullptr;
    }

    int TileMap::getIndex(const glm::ivec2 &pos)
    {
        auto it = std::find_if(mTiles.begin(), mTiles.end(), [=](const Tile &ntile) -> bool
                               { return ntile.x >= pos.x && ntile.x <= pos.x + mTileWidth - 1 && ntile.y >= pos.y && ntile.y <= pos.y + mTileHeight - 1; });

        return it != mTiles.end() ? it - mTiles.begin() : -1;
    }

    void TileMap::removeTile(const glm::ivec2 &mCoords)
    {
        int index = getIndex(mCoords);
        if (index > -1)
        {
            mTiles.erase(mTiles.begin() + index);
        }
    }

} // namespace Plutus
