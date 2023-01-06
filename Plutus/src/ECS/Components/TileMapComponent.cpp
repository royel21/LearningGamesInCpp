#include "TileMapComponent.h"
#include <algorithm>
#include <Assets/Assets.h>

namespace Plutus
{
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

    Vec4f TileMapComponent::getTexCoord(Tile& tile) {
        return  AssetManager::get()->getAsset<Texture>(mTextures[tile.texture])->getUV(tile.texcoord);
    }

    Vec4f TileMapComponent::getTexCoord(int texId, int coordIndex) {
        return  AssetManager::get()->getAsset<Texture>(mTextures[texId])->getUV(coordIndex);
    }

    Tile* TileMapComponent::getTile(const Vec2i& mCoords)
    {
        int index = getIndex(mCoords);
        return index > -1 ? &mTiles[index] : nullptr;
    }

    int TileMapComponent::getIndex(const Vec2i& pos)
    {
        auto it = std::find_if(mTiles.begin(), mTiles.end(), [=](const Tile& ntile) -> bool
            { return ntile.x == pos.x && ntile.y == pos.y; });

        return it != mTiles.end() ? int(it - mTiles.begin()) : -1;
    }

    bool TileMapComponent::removeTile(const Vec2i& mCoords)
    {
        int index = getIndex(mCoords);
        if (index > -1)
        {
            mTiles.erase(mTiles.begin() + index);
        }
        return index > -1;
    }

} // namespace Plutus
