#pragma once

namespace Plutus
{
    struct TileMapComponent;

    class MapRender
    {
    public:
        MapRender();
        ~MapRender();

        void init();
        void draw();

    private:
        TileMapComponent* tileMap;
    };
} // namespace Plutus