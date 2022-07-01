#pragma once

#include <string>
#include <vector>
#include <array>
#include <Math/Vectors.h>

namespace Plutus
{
    struct Texture;
    struct TileMapComponent;

    struct BaseTile {
        int x = 0;
        int y = 0;
        int texture = 0;
        int texcoord = 0;
    };

    struct Tile : BaseTile
    {
        bool flipX;
        bool flipY;
        float rotate;
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

    struct AnimateTile : BaseTile {
        int frame = 0;
        float delay = 0;
        float currentTime = 0;
        TileAnimation* anim;
    };

    struct TileMapComponent
    {
        int mWidth = 60;
        int mHeight = 34;
        int mLayer = 0;
        std::array<std::string, 16> mTextures;
        std::vector<AnimateTile> mAnimateTiles;
        std::vector<TileAnimation> mTileAnims;
        std::vector<Tile> mTiles;

        TileMapComponent(int layer = 0) { mLayer = layer; }

        void addTexture(int id, const std::string& texture);
        void addTexture(const std::string& texture);
        void removeTexture(int index);

        Texture* getTexture(int id);
        Vec4f getTexCoord(Tile& tile);
        Vec4f getTexCoord(int texId, int coordIndex);

        inline void addTile(Tile& tile) { mTiles.push_back(tile); }

        inline void addTile(int x, int y, int texcoord, unsigned int texture, bool fx = false, bool fy = false, float rotate = 0) {
            mTiles.emplace_back(x, y, texcoord, texture, fx, fy, rotate);
        }

        Tile* getTile(const Vec2i& mCoords);
        int getIndex(const Vec2i& mCoords);
        bool removeTile(const Vec2i& mCoords);

        bool operator < (const TileMapComponent& ref) const {
            return mLayer < ref.mLayer;
        }
        bool operator < (const TileMapComponent* ref) const {
            return mLayer < ref->mLayer;
        }
    };
} // namespace Plutus