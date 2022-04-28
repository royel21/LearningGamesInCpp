#include "TileMapComponent.h"
#include <algorithm>
#include <Assets/Assets.h>

namespace Plutus
{

    vec4f Tile::getRect() {
        return { x * parent->mTileWidth, y * parent->mTileHeight, parent->mTileWidth, parent->mTileHeight };
    }

    void TileMapComponent::addTexture(int id, const std::string& texture)
    {
        mTextures[id] = texture;
    }

    void TileMapComponent::addTexture(const std::string& texture) {
        for (size_t i = 0; i < 16; i++)
        {
            if (mTextures[i].empty()) {
                mTextures[i] = texture;
                break;
            }
        }
    }

    void TileMapComponent::removeTexture(int index)
    {
        mTextures[index] = "";
    }

    Texture* TileMapComponent::getTexture(int id) {
        return  AssetManager::get()->getAsset<Texture>(mTextures[id]);
    }

    void TileMapComponent::addTile(Tile& tile)
    {
        mTiles.push_back(tile);
        mTiles.back().setParent(this);
    }

    Tile* TileMapComponent::getTile(const vec2i& mCoords)
    {
        int index = getIndex(mCoords);
        return index > -1 ? &mTiles[index] : nullptr;
    }

    int TileMapComponent::getIndex(const vec2i& pos)
    {
        auto it = std::find_if(mTiles.begin(), mTiles.end(), [=](const Tile& ntile) -> bool
            { return ntile.x == pos.x && ntile.y == pos.y; });

        return it != mTiles.end() ? int(it - mTiles.begin()) : -1;
    }

    bool TileMapComponent::removeTile(const vec2i& mCoords)
    {
        int index = getIndex(mCoords);
        if (index > -1)
        {
            mTiles.erase(mTiles.begin() + index);
        }
        return index > -1;
    }

    void TileMapComponent::addTile(int pos, int tile) {

        // uint32_t texId = 0xf & tile;
        // int uvIndex = 0xffff & (tile >> 4);
        // uint32_t transform = 0xf & tile;

        // bool flipX = 0x2000000 & tile;
        // bool flipY = 0x4000000 & tile;
        // float rotation = 0x8000000 & tile ? 90.0f : 0;

        // pos

    }

} // namespace Plutus
