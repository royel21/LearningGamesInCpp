#include "SceneSerializer.h"

#include "Serialize.h"
#include <ECS/Components.h>
#include <ECS/Scene.h>
#include <Assets/AssetManager.h>
#include <Core/type.h>

namespace Plutus
{

    void Textures_JSON(Serializer& serializer)
    {
        auto writer = serializer.getWriter();
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

    void Tag_JSON(Serializer& serializer, const Tag& tag)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Tag");
            writer->String("Name");
            writer->String(tag.Name.c_str());
        }
    }

    void Transform_JSON(Serializer& serializer, const Transform& trans)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Transform");
            writer->String("x");
            writer->Double(trans.x);
            writer->String("y");
            writer->Double(trans.y);
            writer->String("h");
            writer->Int(trans.h);
            writer->String("w");
            writer->Int(trans.w);
            writer->String("r");
            writer->Double(trans.r);
        }
        writer->EndObject();
    }

    void Sprite_json(Serializer& serializer, const Sprite& sprite)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Sprite");
            writer->String("texture");
            writer->String(sprite.mTextureId.c_str());
            writer->String("color");
            writer->Int(sprite.mColor.rgba);
            writer->String("mFlipX");
            writer->Bool(sprite.mFlipX);
            writer->String("mFlipY");
            writer->Bool(sprite.mFlipX);
        }
        writer->EndObject();
    }

    void Script_JSON(Serializer& serializer, const Script& script)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Script");
            writer->String("path");
            writer->String(script.path.c_str());
        }
        writer->EndObject();
    }

    void Animate_JSON(Serializer& ser, const Animation& anim)
    {
        auto writer = ser.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Animation");

            writer->String("texture");
            writer->StartArray();
            for (auto tx : anim.mTextures)
            {
                writer->String(tx->name.c_str());
            }
            writer->EndArray();

            writer->String("sequences");
            writer->StartArray();
            for (auto& a : anim.mSequences)
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

    void TileMap_json(Serializer& serializer, const TileMap& tilemap)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("TileMap");
            writer->String("tilewidth");
            writer->Int(tilemap.mTileWidth);
            writer->String("tileheight");
            writer->Int(tilemap.mTileHeight);
            //Array of tileset name
            writer->String("tileset");
            writer->String(tilemap.mTileset->name.c_str());

            //Array of textures
            writer->String("textures");
            writer->StartArray();
            for (auto tex : tilemap.mTextures)
            {
                writer->String(tex->name.c_str());
            }
            writer->EndArray();
            //Tiles Array
            writer->String("tiles");
            writer->StartArray();
            {
                for (auto tile : tilemap.mTiles)
                {
                    //Tile OBJ
                    writer->StartObject();
                    {
                        writer->String("texcoord");
                        writer->Int(tile.texcoord);
                        writer->String("texture");
                        writer->Int(tile.texture);
                        writer->String("x");
                        writer->Int(tile.x);
                        writer->String("y");
                        writer->Int(tile.y);
                        writer->String("flipX");
                        writer->Bool(tile.flipX);
                        writer->String("flipY");
                        writer->Bool(tile.flipY);
                        writer->String("r");
                        writer->Double(tile.rotate);
                        writer->String("color");
                        writer->Int(tile.color);
                    }
                    writer->EndObject();
                }
            }
            writer->EndArray();
        }
        writer->EndObject();
    }

    void sceneSerializer(Serializer& ser, Ref<Scene>& scene)
    {
        auto writer = ser.getWriter();
        writer->StartObject();

        Textures_JSON(ser);

        writer->String("layers");
        writer->StartArray();
        auto layers = scene->getLayers();
        for (auto layer : *layers)
        {
            writer->StartObject();
            {
                writer->String("name");
                writer->String(layer.first.c_str());
                writer->String("entities");
                writer->StartArray();
                {
                    for (auto ent : layer.second.mEntities)
                    {
                        writer->StartObject();
                        writer->String("name");
                        writer->String(ent->getName().c_str());
                        writer->String("components");
                        writer->StartArray();
                        {
                            if (ent->hasComponent<Transform>())
                            {
                                Transform_JSON(ser, ent->getComponent<Transform>());
                            }
                            if (ent->hasComponent<Sprite>())
                            {
                                Sprite_json(ser, ent->getComponent<Sprite>());
                            }
                            if (ent->hasComponent<Animation>())
                            {
                                Animate_JSON(ser, ent->getComponent<Animation>());
                            }
                            if (ent->hasComponent<Script>())
                            {
                                Script_JSON(ser, ent->getComponent<Script>());
                            }
                            if (ent->hasComponent<TileMap>())
                            {
                                TileMap_json(ser, ent->getComponent<TileMap>());
                            }
                        }
                        writer->EndArray();
                        writer->EndObject();
                    }
                }
                writer->EndArray();
            }
            writer->EndObject();
        }
        writer->EndArray();

        writer->EndObject();
    }

} // namespace Plutus
