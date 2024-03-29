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
    constexpr uint32_t FLIPX = 0x2000000;
    constexpr uint32_t FLIPY = 0x4000000;
    constexpr uint32_t ROTATE = 0x8000000;

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

    void loadTileMap(Entity& ent, const rapidjson::Value::Object& value, JsonHelper& jhelper)
    {
        int w = value["width"].GetInt();
        int h = value["height"].GetInt();
        int layer = value["layer"].GetInt();
        auto tmap = ent.addComponent<TileMapComponent>(layer);
        tmap->mWidth = w;
        tmap->mHeight = h;

        for (auto& obj : value["textures"].GetArray())
        {
            tmap->addTexture(obj["id"].GetInt(), obj["name"].GetString());
        }

        if (value.HasMember("animations")) {
            for (auto& obj : value["animations"].GetArray())
            {
                tmap->mTileAnims.push_back({});
                auto& anim = tmap->mTileAnims.back();

                anim.texId = obj["texId"].GetInt();
                anim.duration = obj["duration"].GetFloat();
                for (auto& f : obj["frames"].GetArray())
                {
                    anim.frames.push_back(f.GetInt());
                }
            }
        }

        if (value.HasMember("animateTiles")) {
            for (auto& atile : value["animateTiles"].GetArray())
            {
                tmap->mAnimateTiles.push_back({});
                auto& tanim = tmap->mAnimateTiles.back();

                auto i = atile.GetInt();

                tanim.x = ((i >> 12) % tmap->mWidth);
                tanim.y = (i >> 12) / tmap->mWidth;

                tanim.texture = 0xfff & i;
                tanim.anim = &tmap->mTileAnims[tanim.texture];
                tanim.texcoord = tanim.anim->frames[0];
            }
        }

        auto tiles = value["tiles"].GetArray();
        for (size_t i = 0; i < tiles.Size(); i++)
        {
            uint32_t d = tiles[i].GetUint();
            if (d != 0) {
                int x = (i % tmap->mWidth);
                int y = i / tmap->mWidth;

                uint32_t texId = (0xf & d) - 1;
                int uvIndex = 0xffff & (d >> 4);
                bool flipX = FLIPX & d;
                bool flipY = FLIPY & d;
                float rotation = ROTATE & d ? 90.0f : 0;

                tmap->addTile(x, y, uvIndex, texId, flipX, flipY, rotation);
            }
        }
    }

    bool SceneLoader::loadFromPath(const char* path, Scene* scene)
    {
        auto basedir = AssetManager::get()->getBaseDir();
        std::string ex = Utils::getExtension(path);
        if (ex == "json")
        {
            auto data = FileIO::readFileAsString((basedir + path).c_str());
            if (!data.empty()) {
                return loadFromString(data, scene);
            }
        }
        return false;
    }

    void loadFixtures(PhysicBodyComponent* phyBody, const rapidjson::Value::Object& value, JsonHelper& jhelper) {
        for (auto& val : value["fixtures"].GetArray()) {
            auto obj = val.GetJsonObject();
            jhelper.value = &obj;

            auto& fix = phyBody->addFixture(jhelper.getInt("type"));

            fix.offset = jhelper.getFloat2("offset");
            fix.size = jhelper.getFloat2("size", { {1,1} });
            fix.radius = jhelper.getFloat("radius");
            fix.friction = jhelper.getFloat("frict", 0.3f);
            fix.density = jhelper.getFloat("dens", 1);
            fix.restitution = jhelper.getFloat("restit");
            fix.isSensor = jhelper.getInt("sensor", false);
            fix.mask = jhelper.getInt("mask", 0xffff);
            fix.group = jhelper.getInt("group");
            fix.category = jhelper.getInt("category", 1);
        }
    }

    bool SceneLoader::loadFromString(const std::string& jsonData, Scene* scene)
    {
        bool success = false;
        rapidjson::Document doc;

        if (doc.Parse(jsonData.c_str()).HasParseError() == false) {

            JsonHelper jhelper;
            auto docObj = doc.GetJsonObject();
            jhelper.value = &docObj;

            scene->mBGColor = jhelper.getFloat4("bg-color", { 0,0,0,1 });
            scene->mTileWidth = jhelper.getInt("tile-width", 32);
            scene->mTileHeight = jhelper.getInt("tile-height", 32);

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

            if (doc.HasMember("scripts") && doc["scripts"].IsArray())
            {
                for (auto& script : doc["scripts"].GetArray())
                {
                    auto name = script["id"].GetString();
                    auto path = script["path"].GetString();
                    AssetManager::get()->addAsset<Script>(name, path);
                }
            }

            if (doc.HasMember("sounds") && doc["sounds"].IsArray())
            {
                for (auto& sound : doc["sounds"].GetArray())
                {
                    auto id = sound["id"].GetString();
                    auto path = sound["path"].GetString();
                    int type = sound["type"].GetInt();
                    AssetManager::get()->addAsset<Sound>(id, path, type);
                }
            }

            //Load All Textures
            if (doc.HasMember("textures") && doc["textures"].IsArray())
            {
                auto textures = doc["textures"].GetArray();
                for (uint32_t i = 0; i < textures.Size(); i++)
                {
                    auto tex = textures[i].GetJsonObject();
                    jhelper.value = &tex;

                    auto id = jhelper.getString("id");
                    auto path = jhelper.getString("path");
                    int tilewidth = jhelper.getInt("tileWidth");
                    int tileheight = jhelper.getInt("tileHeight");
                    int spacing = jhelper.getInt("spacing");
                    int margin = jhelper.getInt("margin");
                    int glFilter = jhelper.getInt("gl-filter", GL_NEAREST);
                    int texUnit = jhelper.getInt("tex-unit", 0);
                    auto texture = AssetManager::get()->addAsset<Texture>(id, path, tilewidth, tileheight, glFilter, texUnit);

                    texture->mSpacing = spacing;
                    texture->mMargin = margin;
                    texture->calculateUV();
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
                    bool visible = true;
                    if (entObj.HasMember("visible")) {
                        visible = entObj["visible"].GetInt();
                    }

                    Entity entity = scene->createEntity(name, visible);

                    auto components = entObj["components"].GetArray();
                    for (uint32_t i = 0; i < components.Size(); i++)
                    {
                        auto component = components[i].GetJsonObject();
                        jhelper.value = &component;

                        std::string compType = component["name"].GetString();
                        if (compType == "Transform")
                        {
                            auto trans = entity.addComponent<TransformComponent>();
                            trans->x = jhelper.getFloat("x");
                            trans->y = jhelper.getFloat("y");
                            trans->offsetX = jhelper.getFloat("offset-x");
                            trans->offsetY = jhelper.getFloat("offset-y");
                            trans->w = jhelper.getInt("w");
                            trans->h = jhelper.getInt("h");
                            trans->r = jhelper.getFloat("r");
                            trans->layer = jhelper.getInt("l");
                            trans->sortY = jhelper.getBool("sy");
                            continue;
                        }

                        if (compType == "Sprite")
                        {
                            auto spr = entity.addComponent<SpriteComponent>(component["tex"].GetString());
                            spr->mFlipX = jhelper.getInt("fx");
                            spr->mFlipY = jhelper.getInt("fy");
                            spr->mColor = jhelper.getInt("c");
                            spr->mUVCoord = jhelper.getFloat4("uvc");
                            continue;
                        }
                        if (compType == "Animation")
                        {
                            loadAnimation(entity, components[i].GetJsonObject());
                            continue;
                        }
                        if (compType == "TileMap")
                        {
                            loadTileMap(entity, components[i].GetJsonObject(), jhelper);
                            continue;
                        }
                        if (compType == "Script")
                        {
                            auto script = entity.addComponent<ScriptComponent>(component["script"].GetString());
                        }
                        if (compType == "RigidBody") {
                            auto rbody = entity.addComponent<RigidBodyComponent>((BodyType)jhelper.getInt("type"));

                            rbody->mBullet = jhelper.getInt("isbullet");
                            rbody->mFixedRotation = jhelper.getInt("fixedRotation");
                            rbody->mLinearDamping = jhelper.getFloat("linearDamp");
                            rbody->mGravityScale = jhelper.getFloat("gravScale");
                            rbody->mBodyType = (BodyType)jhelper.getInt("bodyType");
                            rbody->mMaxVel = jhelper.getFloat2("max-vel");
                            rbody->mSpeedReducctionFactor = jhelper.getFloat2("spd-refact", { 0.95f });

                            loadFixtures(rbody, component, jhelper);
                        }

                        if (compType == "PhysicBody") {
                            auto phybody = entity.addComponent<PhysicBodyComponent>();
                            loadFixtures(phybody, component, jhelper);
                        }
                    }
                }
            }
            success = true;
        }
        return success;
    }
} // namespace Plutus
