#include "SceneSerializer.h"

#include "Serialize.h"
#include <ECS/Components.h>
#include <ECS/Scene.h>
#include <Assets/AssetManager.h>
#include <Core/type.h>

namespace Plutus
{

    void Textures_JSON(rapidjson::Writer<rapidjson::StringBuffer>* writer)
    {
        writer->String("textures");
        writer->StartArray();
        for (auto tile : AssetManager::get()->mTextures.mTileSets)
        {
            writer->StartObject();

            writer->String("id");
            writer->String(tile.first.c_str());
            writer->String("path");
            writer->String(tile.second.path.c_str());
            writer->String("columns");
            writer->Int(tile.second.columns);
            writer->String("width");
            writer->Int(tile.second.tileWidth);
            writer->String("height");
            writer->Int(tile.second.tileHeight);

            writer->EndObject();
        }
        writer->EndArray();
    }

    void Tag_JSON(rapidjson::Writer<rapidjson::StringBuffer>* writer, const Tag* tag)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Tag");
            writer->String("Name");
            writer->String(tag->Name.c_str());
        }
    }

    void Transform_JSON(rapidjson::Writer<rapidjson::StringBuffer>* writer, const Transform* trans)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Transform");
            writer->String("x");
            writer->Double(trans->x);
            writer->String("y");
            writer->Double(trans->y);
            writer->String("h");
            writer->Int(trans->h);
            writer->String("w");
            writer->Int(trans->w);
            writer->String("layer");
            writer->Int(trans->layer);
            writer->String("sortY");
            writer->Bool(trans->sortY);
            writer->String("r");
            writer->Double(trans->r);
        }
        writer->EndObject();
    }

    void Sprite_json(rapidjson::Writer<rapidjson::StringBuffer>* writer, const Sprite* sprite)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Sprite");
            writer->String("texture");
            writer->String(sprite->mTextureId.c_str());
            writer->String("color");
            writer->Int(sprite->mColor);
            writer->String("mFlipX");
            writer->Bool(sprite->mFlipX);
            writer->String("mFlipY");
            writer->Bool(sprite->mFlipX);
        }
        writer->EndObject();
    }

    void Script_JSON(rapidjson::Writer<rapidjson::StringBuffer>* writer, const Script* script)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Script");
            writer->String("path");
            writer->String(script->path.c_str());
        }
        writer->EndObject();
    }

    void Animate_JSON(rapidjson::Writer<rapidjson::StringBuffer>* writer, const Animation* anim)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Animation");

            writer->String("texture");
            writer->StartArray();
            for (auto tx : anim->mTextures)
            {
                writer->String(tx.c_str());
            }
            writer->EndArray();

            writer->String("sequences");
            writer->StartArray();
            for (auto& a : anim->mSequences)
            {
                writer->StartObject();
                {
                    writer->String("texIndex");
                    writer->Int(a.second.mTexIndex);
                    writer->String("seqTime");
                    writer->Double(a.second.mSeqTime);
                    writer->String("name");
                    writer->String(a.first.c_str());
                    writer->String("frames");
                    writer->StartArray();
                    for (auto f : a.second.mFrames)
                    {
                        writer->Int(f);
                    }
                    writer->EndArray();
                }
                writer->EndObject();
            }
            writer->EndArray();
        }
        writer->EndObject();
    }

    void TileMap_json(rapidjson::Writer<rapidjson::StringBuffer>* writer, const TileMap* tilemap)
    {
        writer->StartObject();
        {
            writer->String("name");
            writer->String("TileMap");
            writer->String("tilewidth");
            writer->Int(tilemap->mTileWidth);
            writer->String("tileheight");
            writer->Int(tilemap->mTileHeight);
            writer->String("layer");
            writer->Int(tilemap->mLayer);
            //Array of tileset name
            writer->String("tileset");
            writer->String("");


            //Array of textures
            writer->String("textures");
            writer->StartArray();
            for (auto tex : tilemap->mTextures)
            {
                writer->String(tex.second->name.c_str());
            }
            writer->EndArray();
            //Tiles Array
            writer->String("tiles");
            writer->StartArray();
            {
                for (auto tile : tilemap->mTiles)
                {
                    //Tile OBJ
                    writer->StartObject();
                    {
                        writer->String("tc");
                        writer->Int(tile.texcoord);
                        writer->String("txi");
                        writer->Int(tile.texture);
                        writer->String("x");
                        writer->Int(tile.x);
                        writer->String("y");
                        writer->Int(tile.y);
                        writer->String("fx");
                        writer->Int(tile.flipX);
                        writer->String("fy");
                        writer->Int(tile.flipY);
                        writer->String("r");
                        writer->Double(tile.rotate);
                        writer->String("c");
                        writer->Int(tile.color);
                    }
                    writer->EndObject();
                }
            }
            writer->EndArray();
        }
        writer->EndObject();
    }

    std::string sceneSerializer(Ref<Scene>& scene)
    {
        Serializer ser;
        auto writer = ser.getWriter();
        writer->StartObject();

        Textures_JSON(writer);

        writer->String("entities");
        writer->StartArray();
        scene->getRegistry()->each([&](entt::entity e) {
            Plutus::Entity ent = { e, scene.get() };
            writer->StartObject();
            writer->String("name");
            writer->String(ent.getName().c_str());
            writer->String("components");
            writer->StartArray();
            {
                if (ent.hasComponent<Transform>())
                {
                    Transform_JSON(writer, ent.getComponent<Transform>());
                }
                if (ent.hasComponent<Sprite>())
                {
                    Sprite_json(writer, ent.getComponent<Sprite>());
                }
                if (ent.hasComponent<Animation>())
                {
                    Animate_JSON(writer, ent.getComponent<Animation>());
                }
                if (ent.hasComponent<Script>())
                {
                    Script_JSON(writer, ent.getComponent<Script>());
                }
                if (ent.hasComponent<TileMap>())
                {
                    TileMap_json(writer, ent.getComponent<TileMap>());
                }
            }
            writer->EndArray();
            writer->EndObject();
            });
        writer->EndArray();

        writer->EndObject();

        return ser.getString();
    }

} // namespace Plutus
