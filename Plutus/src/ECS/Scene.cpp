#include "Scene.h"
#include "Serialize/Serialize.h"

#include "Graphics/Camera2D.h"
#include "Graphics/GLSL.h"
#include "./Components.h"

#include <Input/Input.h>
#include <memory>
#include <Time/Timer.h>
#include <cstdio>

constexpr float ROW2 = 32;
constexpr float ROW3 = 2 * 32;
constexpr float ROW4 = 3 * 32;
constexpr float ROW6 = 5 * 32;
constexpr float ROW7 = 6 * 32;
constexpr float ROW8 = 7 * 32;
constexpr float ROW9 = 8 * 32;
constexpr float LASTROW = 33 * 32;
constexpr float SPEED = 1;

namespace Plutus
{
    const std::string Entity::getName()
    {
        auto tag = getComponent<Tag>();
        return tag.Name;
    }
    void Entity::setName(const std::string& name)
    {
        auto tag = getComponent<Tag>();
        tag.Name = name;
    }

    Scene::~Scene()
    {
        mRegistry.clear();
        mLayers.clear();
        mShader.dispose();
    }

    void Scene::Init(Camera2D* camera)
    {
        mRenderer.init(camera);
        mCamera = camera;
        mShader.CreateProgWithShader(vertexShader2.c_str(), fragShader2.c_str());
        addLayer("Layer0");
    }

    Entity* Scene::createEntity(const std::string& name)
    {
        auto ent = mCurrentLayer->add(std::make_shared<Entity>(mRegistry.create(), this));
        ent->addComponent<Tag>(name);

        return ent.get();
    }

    void Scene::removeEntity(Entity* ent)
    {
        mRegistry.destroy(ent->mId);
        mCurrentLayer->remove(ent);
    }

    bool Scene::removeLayer(std::string name)
    {
        if (mLayers.size() == 1)
            return false;

        auto found = mLayers.find(name);
        if (found != mLayers.end())
        {
            for (int i = found->second.mEntities.size() - 1; i >= 0; i--)
            {
                removeEntity(found->second.mEntities[i].get());
            }
            mLayers.erase(found);
            return true;
        }
        return false;
    }

    Layer* Scene::addLayer(const std::string& name)
    {
        mLayers[name] = Layer(name);
        mCurrentLayer = &mLayers[name];
        return &mLayers[name];
    }

    Layer* Scene::setLayer(const std::string& name)
    {
        mCurrentLayer = &mLayers[name];
        return &mLayers[name];
    }
    void Scene::enableShader()
    {
        mShader.enable();
        mShader.setUniform1i("hasTexture", 0);
        mShader.setUniform1i("mySampler", 0);
        mShader.setUniformMat4("camera", mCamera->getCameraMatrix());
        glClearDepth(1.0f);
    }

    void Scene::disableShader()
    {
        mShader.disable();
    }

    void Scene::draw()
    {
        enableShader();

        mRenderer.begin();
        for (auto& layer : mLayers)
        {
            if (layer.second.isVisible)
            {
                for (auto ent : layer.second.mEntities)
                {
                    if (ent->hasComponent<TileMap>())
                    {
                        auto& map = ent->getComponent<TileMap>();
                        auto tileset = map.mTextures[0];
                        const int w = map.mTileWidth;
                        const int h = map.mTileHeight;

                        if (map.mTiles.size())
                        {
                            auto start = Timer::millis();
                            std::sort(map.mTiles.begin(), map.mTiles.end(), [](Tile& t1, Tile& t2) -> bool
                                { return t1.texture < t2.texture; });

                            mRenderer.reserve(map.mTiles.size());
                            for (auto& tile : map.mTiles)
                            {
                                auto tileset = map.mTextures[tile.texture];
                                glm::vec4 rect{ tile.x, tile.y, w, h };
                                mRenderer.submit(tileset->mTexture.id, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY);
                            }
                        }
                    }

                    if (ent->hasComponent<Sprite>())
                    {
                        auto& trans = ent->getComponent<Transform>();
                        auto& sprite = ent->getComponent<Sprite>();
                        mRenderer.submit(sprite.mTexId, trans.getRect(), sprite.mUVCoord, sprite.mColor, trans.r, sprite.mFlipX, sprite.mFlipY);
                    }
                }
            }
        }

        mRenderer.end();

        disableShader();
    }

    void Scene::update(float dt)
    {
        auto view = mRegistry.view<Script>();
        view.each([dt](auto& script)
            { script.update(dt); });

        auto view2 = mRegistry.view<Animate>();
        view2.each([dt](auto& anim)
            { anim.update(dt); });

        auto view3 = mRegistry.view<TileMap>();

        // view3.each([dt](TileMap &t)
        //            {
        //                auto speed = dt * SPEED;
        //                for (auto &tile : t.mTiles)
        //                {
        //                    if (tile.y == ROW2 || tile.y == ROW4 || tile.y == ROW6 || tile.y == ROW8)
        //                    {
        //                        tile.x = tile.x - speed;
        //                        if (tile.x < -33)
        //                        {
        //                            tile.x = LASTROW;
        //                        }
        //                    }

        //                    if (tile.y == ROW3 || tile.y == ROW7 || tile.y == ROW9)
        //                    {
        //                        tile.x = tile.x + speed;
        //                        if (tile.x > LASTROW)
        //                        {
        //                            tile.x = -33;
        //                        }
        //                    }
        //                }
        //            });

        // LOG_I("time: {0} {1}", Timer::millis() - start, Timer::millis());

        // auto view2 = mRegistry.view<Transform>();
        // // auto size = mCamera->getViewPortSize();
        // view2.each([dt](auto& trans) {
        //     LOG_I("x: {0} y: {1}", trans.x, trans.y);
        //     });
        //     trans.position.x -= vel.speed.x;
        //     // if (trans.position.y + trans.size..x > size.y || trans.position.y <= 0)
        //     // {
        //     //     vel.speed.y *= -1;
        //     // }
        //     if (vel.speed.x > 0 && trans.position.x + trans.size.x <= 0)
        //     {
        //         trans.position.x = 1568;
        //     }

        //     if (vel.speed.x < 0 && trans.position.x >= size.x)
        //     {
        //         trans.position.x = -1568 + size.x;
        //     }
        //     })
    }

    void Scene::clear()
    {
        mLayers.clear();
        mRegistry.clear();
        addLayer("Layer0");
    }

    const glm::vec2 Scene::getScreen()
    {
        return mCamera->getViewPortSize();
    }
} // namespace Plutus
