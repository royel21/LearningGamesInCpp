#include "SceneLoader.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Assets/AssetManager.h>

#include <Serialize/Serialize.h>
#include <rapidjson/document.h>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    void loadAnimation(Entity& ent, rapidjson::Value::Object value)
    {
        // auto anim = ent.addComponent<Animation>(ent);
    }

    void loadTileMap(Entity& ent, rapidjson::Value::Object value)
    {
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        int layer = value["layer"].GetInt();
        auto tmap = ent.addComponent<TileMap>(w, h, layer);

        for (auto& t : value["textures"].GetArray())
        {
            tmap->addTexture(t.GetString());
        }

        auto tiles = value["tiles"].GetArray();

        for (auto& tile : value["tiles"].GetArray())
        {
            int x = tile["x"].GetInt();
            int y = tile["y"].GetInt();
            int fx = tile["fx"].GetInt();
            int fy = tile["fy"].GetInt();
            int t = tile["tc"].GetInt();
            int tx = tile["txi"].GetInt();
            float rotate = tile["r"].GetFloat();
            int color = tile["c"].GetInt();

            tmap->mTiles.emplace_back(x, y, t, tx, fx, fy, rotate, color);
            tmap->mTiles.back().setParent(tmap);
        }
    }

    bool SceneLoader::loadFromJson(const char* path, Ref<Scene>& scene)
    {
        std::string ex = Utils::getExtension(path);
        if (ex == "json")
        {
            auto data = readFileAsString(path);
            if (!data.empty()) {
                return loadFromString(data, scene);
            }
        }
        return false;
    }

    bool SceneLoader::loadFromString(const std::string& jsonData, Ref<Scene>& scene)
    {
        bool success = false;
        rapidjson::Document doc;
        if (doc.Parse(jsonData.c_str()).HasParseError() == false) {
            //Load All Textures
            if (doc.HasMember("textures") && doc["textures"].IsArray())
            {
                auto textures = doc["textures"].GetArray();
                for (uint32_t i = 0; i < textures.Size(); i++)
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

            if (doc.HasMember("entities") && doc["entities"].IsArray())
            {
                //Get the layers
                auto entities = doc["entities"].GetArray();
                for (int i = entities.Size() - 1; i > -1; i--)
                {
                    auto entObj = entities[i].GetJsonObject();

                    auto name = entObj["name"].GetString();
                    Entity entity = scene->createEntity(name);

                    auto components = entObj["components"].GetArray();
                    for (uint32_t i = 0; i < components.Size(); i++)
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
                            spr->mFlipX = component["mFlipX"].GetBool();
                            spr->mFlipY = component["mFlipY"].GetBool();
                            spr->mColor = component["color"].GetInt();
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
                            auto script = entity.addComponent<Script>();
                            script->init(component["path"].GetString(), entity, scene.get());
                        }
                    }
                }
            }
            success = true;
        }
        return success;
    }
} // namespace Plutus
