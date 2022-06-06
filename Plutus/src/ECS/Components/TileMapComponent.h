#pragma once

#include <string>
#include <vector>
#include <array>
#include <Math/Vectors.h>

namespace Plutus
{
    struct Texture;
    struct TileMapComponent;

    struct Tile
    {
        int x = 0;
        int y = 0;
        bool flipX;
        bool flipY;
        float rotate;
        int texcoord = 0;
        int texture = 0;
        Tile() = default;
        Tile(int _x, int _y, int _texcoord, unsigned int _texture, bool fx = false, bool fy = false, float _rotate = 0)
        {
            x = _x;
            y = _y;

            texcoord = _texcoord;
            texture = _texture;
            flipX = fx;
            flipY = fy;
            rotate = _rotate;
        }

        inline bool operator==(Tile tile)
        {
            return (x == tile.x && y == tile.y && texcoord == tile.texcoord);
        }

        inline bool operator!=(Tile tile)
        {
            return (x != tile.x && y != tile.y && texcoord != tile.texcoord);
        }

        bool operator < (const Tile& t) const {
            return texture < t.texture;
        }
    };

    struct TileAnimation {
        int texId;
        float duration;
        std::vector<uint16_t> frames;
    };

    struct AnimateTile {
        int x = 0;
        int y = 0;
        int frame = 0;
        int coordIndex;
        int animIndex;
        float currentTime = 0;
        TileAnimation* anim;
    };

    struct TileMapComponent
    {
        int mWidth = 60;
        int mHeight = 34;
        int mTileWidth = 32;
        int mTileHeight = 32;
        int mLayer = 0;
        std::vector<Tile> mTiles;
        std::vector<AnimateTile> mAnimateTiles;
        std::array<std::string, 16> mTextures;
        std::vector<TileAnimation> mTileAnims;

        TileMapComponent() = default;
        TileMapComponent(int tileWidth, int tileHeight, int layer = 0) : mTileWidth(tileWidth), mTileHeight(tileHeight), mLayer(layer) {}

        void addTexture(int id, const std::string& texture);
        void addTexture(const std::string& texture);
        void removeTexture(int index);

        Texture* getTexture(int id);
        Vec4f getTexCoord(Tile& tile);
        Vec4f getTexCoord(int texId, int coordIndex);

        void addTile(Tile& tile);
        Tile* getTile(const Vec2i& mCoords);
        int getIndex(const Vec2i& mCoords);
        bool removeTile(const Vec2i& mCoords);

        inline Vec4f getRect(float x, float y) {
            return { x * mTileWidth, y * mTileHeight, mTileWidth, mTileHeight };
        }

        inline Vec4f getRect(Tile tile) {
            return { tile.x * mTileWidth, tile.y * mTileHeight, mTileWidth, mTileHeight };
        }

        bool operator < (const TileMapComponent& ref) const {
            return mLayer < ref.mLayer;
        }
        bool operator < (const TileMapComponent* ref) const {
            return mLayer < ref->mLayer;
        }
    };
} // namespace Plutus