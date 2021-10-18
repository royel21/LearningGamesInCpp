#pragma once

#include <cstring>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Plutus
{
    struct Texture;
    struct TileMap;

    struct Tile
    {
        int x = 0;
        int y = 0;
        bool flipX;
        bool flipY;
        float rotate;
        unsigned int color;
        int texcoord = 0;
        int texture = 0;
        TileMap* parent;
        Tile() = default;
        Tile(int _x, int _y, int _texcoord, unsigned int _texture, bool fx = false, bool fy = false, float _rotate = 0, unsigned int _color = 0xffffffff)
        {
            x = _x;
            y = _y;
            flipX = fx;
            flipY = fy;
            texcoord = _texcoord;
            texture = _texture;
            rotate = _rotate;
            color = _color;
        }

        void setParent(TileMap* _parent) {
            parent = _parent;
        }

        glm::vec4 getRect();

        inline bool operator==(Tile tile)
        {
            return (x == tile.x && y == tile.y && texcoord == tile.texcoord);
        }

        inline bool operator!=(Tile tile)
        {
            return (x != tile.x && y != tile.y && texcoord != tile.texcoord);
        }
    };

    struct TileMap
    {
        int mTileWidth;
        int mTileHeight;
        int mLayer = 0;
        std::vector<Tile> mTiles;
        std::unordered_map<int, std::string> mTextures;

        TileMap() = default;
        // TileMap(const TileMap& tilemap);
        TileMap(int tileWidth, int tileHeight, int _layer = 0) : mTileWidth(tileWidth), mTileHeight(tileHeight) {}

        void addTexture(const std::string& texture);
        void removeTexture(int index);

        Texture* getTexture(int id);

        void addTile(Tile& tile);
        Tile* getTile(const glm::ivec2& mCoords);
        int getIndex(const glm::ivec2& mCoords);
        bool removeTile(const glm::ivec2& mCoords);
    };
} // namespace Plutus