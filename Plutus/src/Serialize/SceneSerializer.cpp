#include "SceneSerializer.h"

#include "Serialize.h"

#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Assets/Assets.h>

namespace Plutus
{

    void Textures_JSON(Serializer& ser)
    {
        ser.StartArr("fonts");
        for (auto& tile : AssetManager::get()->getAssets<Font>())
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

        ser.StartArr("scripts");
        for (auto& script : AssetManager::get()->getAssets<Script>())
        {
            ser.StartObj();
            {
                ser.addString("id", script.first);
                ser.addString("path", script.second->mPath);
            }
            ser.EndObj();
        }
        ser.EndArr();

        ser.StartArr("sounds");
        for (auto& asset : AssetManager::get()->getAssets<Sound>())
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

        ser.StartArr("textures");
        for (auto& tile : AssetManager::get()->getAssets<Texture>())
        {
            auto tex = static_cast<Texture*>(tile.second);
            ser.StartObj();
            {
                ser.addString("id", tile.first);
                ser.addString("path", tex->mPath);
                ser.addInt("tileWidth", tex->mTileWidth);
                ser.addInt("tileHeight", tex->mTileHeight);
                ser.addInt("spacing", tex->mSpacing);
                ser.addInt("margin", tex->mMargin);
                ser.addInt("gl-filter", tex->mGlFilter);
                ser.addInt("tex-unit", tex->mTexureUnit);
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

    void TileMap_json(Serializer& ser, TileMapComponent* tilemap)
    {
        ser.StartObj();
        {
            ser.addString("name", "TileMap");
            ser.addInt("width", tilemap->mWidth);
            ser.addInt("height", tilemap->mHeight);
            ser.addInt("layer", tilemap->mLayer);

            //Array of textures
            ser.StartArr("textures");
            for (size_t i = 0; i < 16; i++)
            {
                if (!tilemap->mTextures[i].empty()) {
                    ser.StartObj();
                    ser.addInt("id", i);
                    ser.addString("name", tilemap->mTextures[i].c_str());
                    ser.EndObj();
                }
            }
            ser.EndArr();
            //Animations
            ser.StartArr("animations");
            for (auto& anim : tilemap->mTileAnims)
            {
                ser.StartObj();
                ser.addInt("texId", anim.texId);
                ser.addFloat("duration", anim.duration);
                ser.StartArr("frames");
                for (auto f : anim.frames)
                    ser.addInt(f);
                ser.EndArr();

                ser.EndObj();
            }
            ser.EndArr();

            ser.StartArr("animateTiles");
            for (auto& anim : tilemap->mAnimateTiles)
            {
                int index = (anim.y * tilemap->mWidth) + anim.x;
                ser.addInt((index << 12) | anim.texture);
            }
            ser.EndArr();

            //Tiles Array
            ser.StartArr("tiles");
            {
                for (int y = 0; y < tilemap->mHeight; y++)
                {
                    for (int x = 0; x < tilemap->mWidth; x++)
                    {
                        auto tile = tilemap->getTile(Vec2i{ x , y });
                        if (tile) {
                            int t = tile->texture + 1;
                            t |= tile->texcoord << 4;
                            t |= tile->flipX << 25;
                            t |= tile->flipY << 26;
                            t |= (tile->rotate != 0) << 27;
                            ser.addInt(t);
                        }
                        else {
                            ser.addInt(0);
                        }
                    }
                }
            }
            ser.EndArr();
        }
        ser.EndObj();
    }

    void addFixtures(Serializer& ser, const PhysicBodyComponent* body) {
        ser.StartArr("fixtures");
        {
            for (auto& fix : body->mFixtures)
            {
                ser.StartObj();
                {
                    ser.addInt("type", fix.type);
                    ser.add2Float("offset", fix.offset);

                    if (fix.type != 1) {
                        ser.add2Float("size", fix.size);
                    }
                    else {
                        ser.addFloat("radius", fix.radius);
                    }

                    ser.addFloat("frict", fix.friction);
                    if (fix.type > 0) {
                        ser.addFloat("dens", fix.density);
                        ser.addFloat("restit", fix.restitution);
                    }
                    ser.addInt("mask", fix.mask);
                    ser.addInt("group", fix.group);
                    ser.addInt("category", fix.category);
                    ser.addInt("sensor", fix.isSensor);
                }
                ser.EndObj();
            }
        }
        ser.EndArr();
    }

    std::string SceneSerializer(Scene* scene)
    {
        Serializer ser;
        auto writer = ser.getWriter();
        ser.StartObj();

        ser.addString("format", "Plutus");
        ser.addInt("type", 0);

        ser.add4Float("bg-color", scene->mBGColor);
        ser.addInt("tile-width", scene->mTileWidth);
        ser.addInt("tile-height", scene->mTileHeight);

        Textures_JSON(ser);

        ser.StartArr("entities");
        scene->getRegistry()->each([&](entt::entity e) {
            Plutus::Entity ent = { e, scene };
            auto tag = ent.getComponent<TagComponent>();
            ser.StartObj();
            ser.addString("name", tag->Name);
            ser.addInt("visible", tag->Visible);

            ser.StartArr("components");
            {
                if (ent.hasComponent<AnimationComponent>())
                {
                    Animate_JSON(ser, ent.getComponent<AnimationComponent>());
                }

                if (ent.hasComponent<TransformComponent>())
                {
                    auto trans = ent.getComponent<TransformComponent>();
                    ser.StartObj();
                    {
                        ser.addString("name", "Transform");
                        ser.addFloat("x", trans->x);
                        ser.addFloat("y", trans->y);
                        ser.addFloat("offset-x", trans->offsetX);
                        ser.addFloat("offset-y", trans->offsetY);
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
                    auto rbody = ent.getComponent<RigidBodyComponent>();

                    ser.StartObj();
                    {
                        ser.addString("name", "RigidBody");
                        ser.addInt("isbullet", rbody->mBullet);
                        ser.addInt("fixedRotation", rbody->mFixedRotation);
                        ser.addFloat("linearDamp", rbody->mLinearDamping);
                        ser.addFloat("gravScale", rbody->mGravityScale);
                        ser.addInt("bodyType", rbody->mBodyType);
                        ser.add2Float("max-vel", rbody->mMaxVel);
                        ser.add2Float("spd-refact", rbody->mSpeedReducctionFactor);
                        addFixtures(ser, rbody);

                    }
                    ser.EndObj();
                }

                if (ent.hasComponent<PhysicBodyComponent>()) {
                    auto pbody = ent.getComponent<PhysicBodyComponent>();
                    ser.StartObj();
                    {
                        ser.addString("name", "PhysicBody");
                        addFixtures(ser, pbody);

                    }
                    ser.EndObj();
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
