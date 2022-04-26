#include <stdio.h>
#include <string>
#include <cstring>

#include <iostream>
#include <sstream>

#include <Assets/AssetManager.h>
#include <ECS/Components/TileMapComponent.h>
#include <Serialize/Serialize.h>
#include <cstring>

#include <ECS/Scene.h>
#include <Assets/Assets.h>

using namespace Plutus;

void parseTexture(rapidjson::Value& value) {
    if (value.IsArray()) {
        auto manager = AssetManager::get();
        for (const auto& v : value.GetArray()) {
            manager->addAsset<Texture>(
                v["name"].GetString(),
                v["path"].GetString(),
                v["tileWidth"].GetInt(),
                v["tileHeight"].GetInt()
                );
        }
    }
}

void readMapData(const std::string& path, Scene* scene, int width = 0, int height = 0) {
    rapidjson::Document doc;
    if (loadJsonFromFile(path.c_str(), doc)) {
        if (doc.HasMember("format") && strcmp(doc["format"].GetString(), "Plutus") == 0) {

            // if (doc.HasMember("tilesets") && doc["tilesets"].IsArray()) {
            //     parseTexture(doc["tilesets"]);
            // }

            auto layers = doc["layers"].GetArray();
            for (size_t layerIndex = 0; layerIndex < layers.Size(); layerIndex++)
            {
                auto& layer = layers[layerIndex].GetJsonObject();
                if (layer["type"].GetInt() == 0) {
                    auto ent = scene->createEntity(layer["name"].GetString());
                    auto map = ent.addComponent<TileMapComponent>();
                    map->mLayer = layerIndex;

                    for (const auto& tileset : doc["tilesets"].GetArray()) {
                        map->addTexture(tileset["id"].GetInt(), tileset["name"].GetString());
                    }

                    auto& data = layer["data"].GetArray();

                    auto countX = layer["width"].GetInt();
                    auto countY = layer["height"].GetInt();

                    for (size_t i = 0; i < data.Size(); i++)
                    {
                        int x = (i % countX);
                        int y = (countY - 1) - (i / countY);

                        uint32_t d = data[i].GetUint();
                        uint32_t texId = 0xf & d;
                        int uvIndex = 0xffff & (d >> 4);
                        uint32_t transform = 0xf & d;

                        bool flipX = 0x2000000 & d;
                        bool flipY = 0x4000000 & d;
                        float rotation = 0x8000000 & d ? 90.0f : 0;
                        map->addTile(Tile{ x,y, uvIndex, texId, flipX, flipY, rotation });
                    }// end Tiles
                }// end layer
            }// end layers
        }
    }
}

int main(int argc, char** argv)
{
    Scene scene;

    readMapData("assets/tilemaps/testing.json", &scene);
    auto ent = scene.getEntityByName("bg1");

    if (ent) {
        auto map = ent.getComponent<TileMapComponent>();
        printf("size: %i", map->mTiles.size());
    }

    return 0;
}