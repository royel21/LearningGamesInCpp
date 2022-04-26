#pragma once

#include <string>
#include <vector>
#include <unordered_map>
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
        TileMapComponent* parent;
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

        void setParent(TileMapComponent* _parent) {
            parent = _parent;
        }

        vec4f getRect();

        inline bool operator==(Tile tile)
        {
            return (x == tile.x && y == tile.y && texcoord == tile.texcoord);
        }

        inline bool operator!=(Tile tile)
        {
            return (x != tile.x && y != tile.y && texcoord != tile.texcoord);
        }
    };

    struct TileMapComponent
    {
        int mTileWidth;
        int mTileHeight;
        int mLayer = 0;
        std::vector<Tile> mTiles;
        std::unordered_map<int, std::string> mTextures;

        TileMapComponent() = default;
        // TileMap(const TileMap& tilemap);
        TileMapComponent(int tileWidth, int tileHeight, int _layer = 0) : mTileWidth(tileWidth), mTileHeight(tileHeight) {}

        void addTexture(int id, const std::string& texture);
        void removeTexture(int index);

        Texture* getTexture(int id);

        void addTile(Tile& tile);
        Tile* getTile(const vec2i& mCoords);
        int getIndex(const vec2i& mCoords);
        bool removeTile(const vec2i& mCoords);
    };
} // namespace Plutus