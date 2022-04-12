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

    void TileMapComponent::removeTexture(int index)
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

    Texture* TileMapComponent::getTexture(int id) {
        auto it = mTextures.find(id);
        auto texId = it != mTextures.end() ? it->second : "";
        return  AssetManager::get()->getAsset<Texture>(texId);
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

} // namespace Plutus
