#include "SceneLoader.h"
#include <string>

#include "rapidjson/document.h"

#include "Utils/Utils.h"
#include "ECS/Scene.h"
#include "ECS/Components.h"
#include <cstdio>
#include <Assets/Textures.h>

namespace Plutus
{
    void loadAnimation(Entity *ent, rapidjson::Value::Object value)
    {
        auto &anim = ent->addComponent<Animation>(ent);
    }

    void loadTileMap(Entity *ent, rapidjson::Value::Object value)
    {
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        auto tileset = value["tileset"].GetString();
        auto &tmap = ent->addComponent<TileMap>(w, h);
        tmap.mTileset = Textures::get()->getTexture(tileset);
        auto textures = value["textures"].GetArray();
        for (auto &t : value["textures"].GetArray())
        {
            tmap.mTextures.push_back(Textures::get()->getTexture(t.GetString()));
        }
        auto tiles = value["tiles"].GetArray();
        // LOG_I("tiles:{0}", tiles.Size());
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

    bool SceneLoader::loadFromJson(const char *path, Ref<Scene> &scene)
    {
        bool success = false;
        rapidjson::Document doc;
        if (Utils::loadJson(path, &doc))
        {
            //Load All Textures
            if (doc["textures"].IsArray())
            {
                Textures::get()->cleanUp();

                auto textures = doc["textures"].GetArray();
                for (size_t i = 0; i < textures.Size(); i++)
                {
                    auto tex = textures[i].GetJsonObject();
                    auto id = tex["id"].GetString();
                    auto path = tex["path"].GetString();
                    int columns = tex["columns"].GetInt();
                    int width = tex["width"].GetInt();
                    int height = tex["height"].GetInt();
                    Textures::get()->addTexture(id, path, columns, width, height);
                }
            }

            if (doc["layers"].IsArray())
            {

                //Get the layers
                auto layers = doc["layers"].GetArray();
                for (size_t i = 0; i < layers.Size(); i++)
                {
                    auto objLayer = layers[i].GetJsonObject();

                    auto layerName = objLayer["name"].GetString();
                    auto layer = scene->addLayer(layerName);

                    //get the entities
                    auto entities = objLayer["entities"].GetArray();
                    for (size_t i = 0; i < entities.Size(); i++)
                    {
                        auto objEntity = entities[i].GetJsonObject();
                        auto entityName = objEntity["name"].GetString();
                        auto entity = scene->createEntity(entityName);

                        auto components = objEntity["components"].GetArray();
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
                                entity->addComponent<Transform>(x, y, w, h, r);
                                continue;
                            }
                            if (compType == "Sprite")
                            {
                                auto spr = entity->addComponent<Sprite>(component["texture"].GetString());
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
                                entity->addComponent<Script>(component["path"].GetString(), entity, scene.get());
                            }
                        }
                    }
                }

                auto nlayers = scene->getLayers();

                if (layers.Size() > 0)
                {
                    auto name = nlayers->begin()->first;
                    scene->setLayer(name);
                }
                success = true;
            }
        }
        return success;
    }
} // namespace Plutus
