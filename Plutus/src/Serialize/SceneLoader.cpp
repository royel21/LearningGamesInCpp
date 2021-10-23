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
    void loadAnimation(Entity& ent, rapidjson::Value::Object& value)
    {
        auto anim = ent.addComponent<Animation>();
        if (value.HasMember("sequences")) {
            auto arr = value["sequences"].GetArray();

            if (arr.Size()) {
                for (auto& a : arr) {
                    auto name = a["name"].GetString();
                    Sequence seq;
                    seq.mTexId = a["texId"].GetString();
                    seq.mSeqTime = a["seqTime"].GetFloat();
                    for (auto& f : a["frames"].GetArray()) {
                        seq.mFrames.push_back(f.GetInt());
                    }
                    anim->addSequence(name, seq);
                }
            }
        }
    }

    void loadTileMap(Entity& ent, rapidjson::Value::Object& value)
    {
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        int layer = value["layer"].GetInt();
        auto tmap = ent.addComponent<TileMap>(w, h, layer);

        for (auto& obj : value["textures"].GetArray())
        {
            tmap->addTexture(obj["id"].GetInt(), obj["tex"].GetString());
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

    bool SceneLoader::loadFromJson(const char* path, Scene* scene)
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

    bool SceneLoader::loadFromString(const std::string& jsonData, Scene* scene)
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
                    int tilewidth = tex["width"].GetInt();
                    int tileheight = tex["height"].GetInt();
                    AssetManager::get()->mTextures.addTexture(id, path, columns, tilewidth, tileheight);
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
                            int layer = component["l"].GetInt();
                            bool sortY = component["sy"].GetBool();
                            entity.addComponent<Transform>(x, y, w, h, r, layer, sortY);
                            continue;
                        }
                        if (compType == "Sprite")
                        {
                            auto spr = entity.addComponent<Sprite>(component["tex"].GetString());
                            spr->mFlipX = component["fx"].GetInt();
                            spr->mFlipY = component["fy"].GetInt();
                            spr->mColor = component["c"].GetInt();
                            auto uvs = component["uvc"].GetArray();
                            spr->mUVCoord = { uvs[0].GetFloat(),uvs[1].GetFloat(),uvs[2].GetFloat(),uvs[3].GetFloat() };
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
                            auto script = entity.addComponent<Script>(component["script"].GetString());
                        }
                    }
                }
            }
            success = true;
        }
        return success;
    }
} // namespace Plutus
