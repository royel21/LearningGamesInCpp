#include "SceneLoader.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Assets/Assets.h>

#include <Serialize/Serialize.h>
#include <rapidjson/document.h>

#include <Utils/Utils.h>
#include <Utils/FileIO.h>

namespace Plutus
{
    void loadAnimation(Entity& ent, const rapidjson::Value::Object& value)
    {
        auto anim = ent.addComponent<AnimationComponent>();
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

    void loadTileMap(Entity& ent, const rapidjson::Value::Object& value)
    {
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        int layer = value["layer"].GetInt();
        auto tmap = ent.addComponent<TileMapComponent>(w, h, layer);

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

    bool SceneLoader::loadFromPath(const char* path, Scene* scene)
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

            JsonHelper jhelper;
            auto docObj = doc.GetJsonObject();
            jhelper.value = &docObj;

            scene->setGravity(jhelper.getFloat2("grv", { 0.0f, -9.8f }));
            scene->setTimeIterSec(jhelper.getFloat("fps", 60));
            scene->setVelIter(jhelper.getInt("vel-itel", 8));
            scene->setPositionIter(jhelper.getInt("pos-itel", 3));
            scene->setAutoClearForce(jhelper.getBool("cls-forc", true));

            if (doc.HasMember("fonts") && doc["fonts"].IsArray())
            {
                auto sounds = doc["fonts"].GetArray();
                for (uint32_t i = 0; i < sounds.Size(); i++)
                {
                    auto tex = sounds[i].GetJsonObject();
                    auto id = tex["id"].GetString();
                    auto path = tex["path"].GetString();
                    int size = tex["size"].GetInt();
                    AssetManager::get()->addAsset<Font>(id, path, size);
                }
            }

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
                    AssetManager::get()->addAsset<Texture>(id, path, columns, tilewidth, tileheight);
                }
            }

            if (doc.HasMember("sounds") && doc["sounds"].IsArray())
            {
                auto sounds = doc["sounds"].GetArray();
                for (uint32_t i = 0; i < sounds.Size(); i++)
                {
                    auto tex = sounds[i].GetJsonObject();
                    auto id = tex["id"].GetString();
                    auto path = tex["path"].GetString();
                    int type = tex["type"].GetInt();
                    AssetManager::get()->addAsset<Sound>(id, path, type);
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
                            entity.addComponent<TransformComponent>(x, y, w, h, r, layer, sortY);
                            continue;
                        }
                        if (compType == "Sprite")
                        {
                            auto spr = entity.addComponent<SpriteComponent>(component["tex"].GetString());
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
                            auto script = entity.addComponent<ScriptComponent>(component["script"].GetString());
                        }
                        if (compType == "RigidBody") {
                            auto rbody = entity.addComponent<RigidBodyComponent>();
                            jhelper.value = &component;

                            rbody->mBullet = jhelper.getInt("b");
                            rbody->mFixedRotation = jhelper.getInt("fr");
                            rbody->mLinearDamping = jhelper.getFloat("ld");
                            rbody->mGravityScale = jhelper.getFloat("gs");
                            rbody->mBodyType = (BodyType)jhelper.getInt("bt");
                            rbody->mOffset = jhelper.getFloat2("o");
                            rbody->mMaxVel = jhelper.getFloat2("maxv");

                            for (auto& val : component["fixs"].GetArray()) {
                                auto obj = val.GetJsonObject();
                                jhelper.value = &obj;

                                auto& fix = rbody->addFixture(jhelper.getInt("type"));

                                fix.offset = jhelper.getFloat2("offset");
                                fix.size = jhelper.getFloat2("size");
                                fix.radius = jhelper.getFloat("radiu");
                                fix.friction = jhelper.getFloat("frict");
                                fix.density = jhelper.getFloat("dens");
                                fix.restitution = jhelper.getFloat("restit");
                                fix.isSensor = jhelper.getInt("sensor");
                            }
                        }
                    }
                }
            }
            success = true;
        }
        return success;
    }
} // namespace Plutus
