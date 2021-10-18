#include "SceneSerializer.h"

#include "Serialize.h"

#include <ECS/Scene.h>
#include <Core/type.h>
#include <ECS/Components.h>
#include <Assets/AssetManager.h>

namespace Plutus
{

    void Textures_JSON(Serializer& ser)
    {
        ser.StartArr("textures");
        for (auto tile : AssetManager::get()->mTextures.mTileSets)
        {
            ser.StartObj();
            {
                ser.addString("id", tile.first);
                ser.addString("path", tile.second.path);
                ser.addInt("columns", tile.second.columns);
                ser.addInt("width", tile.second.tileWidth);
                ser.addInt("height", tile.second.tileHeight);
            }
            ser.EndObj();
        }
        ser.EndArr();
    }


    void Animate_JSON(Serializer& ser, const Animation* anim)
    {
        ser.StartObj();
        {
            ser.addString("name", "Animation");
            ser.StartArr("texture");
            {
                for (auto tx : anim->mTextures)
                {
                    ser.addString(tx);
                }
            }
            ser.EndArr();
            ser.StartArr("sequences");
            {
                for (auto seq : anim->mSequences)
                {
                    ser.StartObj();
                    {
                        ser.addString("name", seq.first);
                        ser.addInt("texIndex", seq.second.mTexIndex);
                        ser.addFloat("seqTime", seq.second.mSeqTime);
                        ser.StartArr("frames");
                        {
                            for (auto f : seq.second.mFrames)
                            {
                                ser.addInt(f);
                            }
                        }
                        ser.EndArr();
                    }
                    ser.EndObj();
                }
            }
            ser.EndArr();
        }
        ser.EndObj();
    }

    void TileMap_json(Serializer& ser, const TileMap* tilemap)
    {
        ser.StartObj();
        {
            ser.addString("name", "TileMap");
            ser.addInt("tilewidth", tilemap->mTileWidth);
            ser.addInt("tileheight", tilemap->mTileHeight);
            ser.addInt("layer", tilemap->mLayer);

            //Array of textures
            ser.StartArr("textures");
            for (auto tex : tilemap->mTextures)
            {
                ser.addString(tex.second->name);
            }
            ser.EndArr();
            //Tiles Array
            ser.StartArr("tiles");
            {
                for (auto tile : tilemap->mTiles)
                {
                    //Tile OBJ
                    ser.StartObj();
                    {
                        ser.addInt("tc", tile.texcoord);
                        ser.addInt("txi", tile.texture);
                        ser.addInt("x", tile.x);
                        ser.addInt("y", tile.y);
                        ser.addInt("fx", tile.flipX);
                        ser.addInt("fy", tile.flipY);
                        ser.addFloat("r", tile.rotate);
                        ser.addInt("c", tile.color);
                    }
                    ser.EndObj();
                }
            }
            ser.EndArr();
        }
        ser.EndObj();
    }

    std::string SceneSerializer(Ref<Scene>& scene)
    {
        Serializer ser;
        auto writer = ser.getWriter();
        ser.StartObj();

        Textures_JSON(ser);

        ser.StartArr("entities");
        scene->getRegistry()->each([&](entt::entity e) {
            Plutus::Entity ent = { e, scene.get() };
            ser.StartObj();
            ser.addString("name", ent.getName());

            ser.StartArr("components");
            {
                if (ent.hasComponent<Transform>())
                {
                    auto trans = ent.getComponent<Transform>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Transform");
                        ser.addFloat("x", trans->x);
                        ser.addFloat("y", trans->y);
                        ser.addInt("w", trans->w);
                        ser.addInt("h", trans->h);
                        ser.addFloat("r", trans->r);
                        ser.addInt("layer", trans->layer);
                        ser.addBool("sortY", trans->sortY);
                    }
                    ser.EndObj();
                }
                if (ent.hasComponent<Sprite>())
                {
                    auto sprite = ent.getComponent<Sprite>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Sprite");
                        ser.addString("texture", sprite->mTextureId);
                        ser.addInt("uvi", sprite->mUvIndex);
                        ser.addInt("color", sprite->mColor);
                        ser.addInt("flipX", sprite->mFlipX);
                        ser.addInt("flipY", sprite->mFlipY);
                        ser.StartArr("uvc");
                        {
                            ser.addFloat(sprite->mUVCoord.x);
                            ser.addFloat(sprite->mUVCoord.y);
                            ser.addFloat(sprite->mUVCoord.z);
                            ser.addFloat(sprite->mUVCoord.w);
                        }
                        ser.EndArr();
                    }
                    ser.EndObj();
                }
                if (ent.hasComponent<Animation>())
                {
                    Animate_JSON(ser, ent.getComponent<Animation>());
                }
                if (ent.hasComponent<Script>())
                {
                    auto script = ent.getComponent<Script>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Script");
                        ser.addString("path", script->path);
                    }
                    ser.EndObj();
                }
                if (ent.hasComponent<TileMap>())
                {
                    TileMap_json(ser, ent.getComponent<TileMap>());
                }
            }
            ser.EndArr();
            ser.EndObj();
            });
        ser.EndArr();
        ser.EndObj();

        return ser.getString();
    }

} // namespace Plutus
