#include "SceneLoader.h"
#include <string>

#include "ECS/Scene.h"
#include "ECS/Components.h"

#include <Assets/AssetManager.h>

#include <Serialize/Serialize.h>
#include <rapidjson/document.h>

namespace Plutus
{
    void loadAnimation(Entity& ent, rapidjson::Value::Object value)
    {
        auto& anim = ent.addComponent<Animation>(ent);
    }

    void loadTileMap(Entity& ent, rapidjson::Value::Object value)
    {
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        int layer = value["layer"].GetInt();
        auto tileset = value["tileset"].GetString();
        auto& tmap = ent.addComponent<TileMap>(w, h, layer);

        tmap.mTileset = AssetManager::get()->mTextures.getTexture(tileset);
        auto textures = value["textures"].GetArray();

        for (auto& t : value["textures"].GetArray())
        {
            tmap.mTextures.push_back(AssetManager::get()->mTextures.getTexture(t.GetString()));
        }

        auto tiles = value["tiles"].GetArray();

        for (size_t i = 0; i < tiles.Size(); i++)
        {
            auto tile = tiles[i].GetJsonObject();
            int x = tile["x"].GetInt();
            int y = tile["y"].GetInt();
            int fx = tile["flipX"].GetBool();
            int fy = tile["flipY"].GetBool();
            int t = tile["texcoord"].GetInt();
            int tx = tile["texture"].GetInt();
            float rotate = tile["r"].GetFloat();
            int color = tile["color"].GetInt();

            tmap.mTiles.emplace_back(x, y, t, tx, fx, fy, rotate, color);
        }
    }

    bool SceneLoader::loadFromJson(const char* path, Ref<Scene>& scene)
    {
        bool success = false;
        rapidjson::Document doc;
        if (loadJson(path, &doc))
        {
            //Load All Textures
            if (doc["textures"].IsArray())
            {
                auto textures = doc["textures"].GetArray();
                for (size_t i = 0; i < textures.Size(); i++)
                {
                    auto tex = textures[i].GetJsonObject();
                    auto id = tex["id"].GetString();
                    auto path = tex["path"].GetString();
                    int columns = tex["columns"].GetInt();
                    int width = tex["width"].GetInt();
                    int height = tex["height"].GetInt();
                    AssetManager::get()->mTextures.addTexture(id, path, columns, width, height);
                }
            }

            if (doc["entities"].IsArray())
            {
                //Get the layers
                auto entities = doc["entities"].GetArray();
                for (size_t i = 0; i < entities.Size(); i++)
                {
                    auto entObj = entities[i].GetJsonObject();

                    auto name = entObj["name"].GetString();
                    auto entity = scene->createEntity(name);

                    auto components = entObj["components"].GetArray();
                    for (size_t i = 0; i < components.Size(); i++)
                    {
                        auto component = components[i].GetJsonObject();
                        std::string compType = component["name"].GetString();
                        if (compType == "Transform")
                        {
                            float x = component["x"].GetFloat();
                            float y = component["y"].GetFloat();
                            int w = component["w"].GetInt();
                            int h = component["h"].GetInt();
                            float r = component["r"].GetFloat();
                            int layer = component["layer"].GetInt();
                            bool sortY = component["sortY"].GetBool();
                            entity.addComponent<Transform>(x, y, w, h, r, layer, sortY);
                            continue;
                        }
                        if (compType == "Sprite")
                        {
                            auto spr = entity.addComponent<Sprite>(component["texture"].GetString());
                            spr.mFlipX = component["mFlipX"].GetBool();
                            spr.mFlipY = component["mFlipY"].GetBool();
                            spr.mColor.setColor(component["color"].GetInt());
                            continue;
                        }
                        if (compType == "Animation")
                        {
                            loadAnimation(entity, components[i].GetJsonObject());
                            continue;
                        }
                        if (compType == "TileMap")
                        {
                            loadTileMap(entity, components[i].GetJsonObject());
                            continue;
                        }
                        if (compType == "Script")
                        {
                            entity.addComponent<Script>(component["path"].GetString(), entity, scene.get());
                        }
                    }
                }

                success = true;
            }
        }
        return success;
    }
} // namespace Plutus
