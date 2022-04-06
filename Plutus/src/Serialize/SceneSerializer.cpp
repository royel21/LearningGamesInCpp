#include "SceneSerializer.h"

#include "Serialize.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Assets/temp/Assets.h>

namespace Plutus
{

    void Textures_JSON(Serializer& ser)
    {
        ser.StartArr("fonts");
        for (auto& tile : AssetManager2::get()->getAssets<Font>())
        {
            auto tex = static_cast<Font*>(tile.second);
            ser.StartObj();
            {
                ser.addString("id", tile.first);
                ser.addString("path", tex->mPath);
                ser.addInt("size", tex->mSize);
            }
            ser.EndObj();
        }
        ser.EndArr();

        ser.StartArr("textures");
        for (auto& tile : AssetManager2::get()->getAssets<Texture2>())
        {
            auto tex = static_cast<Texture2*>(tile.second);
            ser.StartObj();
            {
                ser.addString("id", tile.first);
                ser.addString("path", tex->mPath);
                ser.addInt("columns", tex->mColumns);
                ser.addInt("width", tex->mTileWidth);
                ser.addInt("height", tex->mTileHeight);
            }
            ser.EndObj();
        }
        ser.EndArr();

        ser.StartArr("sounds");
        for (auto& asset : AssetManager2::get()->getAssets<Sound>())
        {
            auto sound = static_cast<Sound*>(asset.second);
            ser.StartObj();
            {
                ser.addString("id", asset.first);
                ser.addString("path", sound->mPath);
                ser.addInt("type", sound->mType);
            }
            ser.EndObj();
        }
        ser.EndArr();
    }


    void Animate_JSON(Serializer& ser, const AnimationComponent* anim)
    {
        ser.StartObj();
        {
            ser.addString("name", "Animation");
            ser.StartArr("sequences");
            {
                for (auto seq : anim->mSequences)
                {
                    ser.StartObj();
                    {
                        ser.addString("name", seq.first);
                        ser.addString("texId", seq.second.mTexId);
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

    void TileMap_json(Serializer& ser, const TileMapComponent* tilemap)
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
                ser.StartObj();
                ser.addInt("id", tex.first);
                ser.addString("tex", tex.second.c_str());
                ser.EndObj();
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

    void RigidBody_json(Serializer& ser, const RigidBodyComponent* rbody) {
        ser.StartObj();
        {
            ser.addString("name", "RigidBody");
            ser.addInt("b", rbody->mBullet);
            ser.addInt("f", rbody->mFixedRotation);
            ser.addFloat("l", rbody->mLinearDamping);
            ser.addFloat("gs", rbody->mGravityScale);
            ser.addInt("bt", rbody->mBodyType);
            ser.add2Float("o", rbody->mOffset);
            ser.StartArr("fixs");
            {
                for (auto& fix : rbody->mFixtures)
                {
                    ser.StartObj();
                    {
                        ser.addInt("type", fix.type);
                        ser.add2Float("offset", fix.offset);
                        ser.add2Float("size", fix.size);
                        ser.addFloat("radiu", fix.radius);
                        ser.addFloat("frict", fix.friction);
                        ser.addFloat("dens", fix.density);
                        ser.addFloat("restit", fix.restitution);
                        ser.addInt("sensor", fix.isSensor);
                    }
                    ser.EndObj();
                }
            }
            ser.EndArr();
        }
        ser.EndObj();
    }

    std::string SceneSerializer(Scene* scene)
    {
        Serializer ser;
        auto writer = ser.getWriter();
        ser.StartObj();

        Textures_JSON(ser);

        ser.StartArr("entities");
        scene->getRegistry()->each([&](entt::entity e) {
            Plutus::Entity ent = { e, scene };
            ser.StartObj();
            ser.addString("name", ent.getName());

            ser.StartArr("components");
            {
                if (ent.hasComponent<TransformComponent>())
                {
                    if (ent.hasComponent<AnimationComponent>())
                    {
                        Animate_JSON(ser, ent.getComponent<AnimationComponent>());
                    }
                    auto trans = ent.getComponent<TransformComponent>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Transform");
                        ser.addFloat("x", trans->x);
                        ser.addFloat("y", trans->y);
                        ser.addInt("w", trans->w);
                        ser.addInt("h", trans->h);
                        ser.addFloat("r", trans->r);
                        ser.addInt("l", trans->layer);
                        ser.addBool("sy", trans->sortY);
                    }
                    ser.EndObj();
                }
                if (ent.hasComponent<SpriteComponent>())
                {
                    auto sprite = ent.getComponent<SpriteComponent>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Sprite");
                        ser.addString("tex", sprite->mTextureId);
                        ser.addInt("c", sprite->mColor);
                        ser.addInt("fx", sprite->mFlipX);
                        ser.addInt("fy", sprite->mFlipY);
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
                if (ent.hasComponent<ScriptComponent>())
                {
                    auto script = ent.getComponent<ScriptComponent>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Script");
                        ser.addString("script", script->mScript);
                    }
                    ser.EndObj();
                }
                if (ent.hasComponent<RigidBodyComponent>()) {
                    RigidBody_json(ser, ent.getComponent<RigidBodyComponent>());
                }
                if (ent.hasComponent<TileMapComponent>())
                {
                    TileMap_json(ser, ent.getComponent<TileMapComponent>());
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
