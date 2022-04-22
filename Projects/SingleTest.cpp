#include <stdio.h>
#include <string>
#include <cstring>

#include <iostream>
#include <sstream>

#include <Assets/AssetManager.h>
#include <ECS/Components/TileMapComponent.h>
#include <Serialize/Serialize.h>
#include <cstring>

using namespace Plutus;



void readMapData(const std::string& path, TileMapComponent* tilemap, int width = 0, int height = 0) {
    rapidjson::Document doc;
    if (loadJsonFromFile(path.c_str(), doc)) {
        if (doc.HasMember("version") && strcmp(doc["version"].GetString(), "1.8") == 0) {
            tilemap->mTileWidth = doc["tilewidth"].GetInt();
            tilemap->mTileHeight = doc["tileheight"].GetInt();


            auto layers = doc["layers"].GetArray();
            for (size_t layerIndex = 0; layerIndex < layers.Size(); layerIndex++)
            {
                auto& layer = layers[layerIndex].GetJsonObject();
                if (strcmp(layer["type"].GetString(), "tilelayer") == 0) {
                    auto props = layer["properties"].GetArray();
                    tilemap->addTexture(layerIndex, props[0]["value"].GetString());

                    int countX = width;
                    int countY = height;

                    if (!countX || !countY) {
                        countX = layer["width"].GetInt();
                        countY = layer["height"].GetInt();
                    }

                    size_t size = countX * countY;
                    auto& data = layer["data"].GetArray();
                    auto dsize = data.Size();
                    if (size > dsize) {
                        size = dsize;
                    }

                    for (size_t i = 0; i < size; i++)
                    {
                        uint32_t d = data[i].GetUint();
                        if (d) {
                            int textCoord = 0xfffffff & d;
                            float rot = 0x1 & (d >> 29) ? 90.0f : 0;
                            bool flipX = 0x1 & (d >> 30);
                            bool flipY = 0x1 & (d >> 31);
                            int x = (i % countX);
                            int y = (countY - 1) - (i / countY);
                            tilemap->addTile(Tile{ x,y, textCoord, (uint32_t)layerIndex, flipX, flipY, rot });
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    TileMapComponent tileMap;

    readMapData("assets/tilemaps/testing.json", &tileMap);
    printf("size: %i", tileMap.mTiles.size());

    return 0;
}