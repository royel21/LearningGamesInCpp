#include "Scene.h"
#include "Serialize/Serialize.h"

#include "Graphics/Camera2D.h"
#include "./Components.h"

#include <Graphics/SpriteBatch2D.h>

#include <Input/Input.h>
#include <memory>
#include <Time/Timer.h>
#include <cstdio>

namespace Plutus
{
    Scene::Scene() {
        addLayer("Layer0");
    }

    Scene::~Scene()
    {
        mRegistry.clear();
        mLayers.clear();
    }

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

    void Scene::draw(SpriteBatch2D* renderbatch)
    {
        // auto  start = Timer::millis();
        // mRegistry.each([&](auto entity) {

        //     });

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
                            std::sort(map.mTiles.begin(), map.mTiles.end(), [](Tile& t1, Tile& t2) -> bool
                                { return t1.texture < t2.texture; });

                            for (auto& tile : map.mTiles)
                            {
                                auto tileset = map.mTextures[tile.texture];
                                glm::vec4 rect{ tile.x, tile.y, w, h };
                                renderbatch->submit(tileset->texId, rect, tileset->getUV(tile.texcoord), { tile.color }, tile.rotate, tile.flipX, tile.flipY);
                            }
                        }
                    }

                    if (ent->hasComponent<Sprite>())
                    {
                        auto& trans = ent->getComponent<Transform>();
                        auto& sprite = ent->getComponent<Sprite>();
                        renderbatch->submit(sprite.getTexId(), trans.getRect(), sprite.mUVCoord, sprite.mColor, trans.r, sprite.mFlipX, sprite.mFlipY, (uint32_t)ent->mId);
                    }
                }
            }
        }
    }

    void Scene::clear()
    {
        mLayers.clear();
        mRegistry.clear();
        addLayer("Layer0");
    }

    Entity* Scene::getEntity(float xPos, float yPos)
    {
        for (auto& ent : mCurrentLayer->mEntities) {
            if (ent->hasComponent<Transform>()) {
                auto& trans = ent->getComponent<Transform>();
                if (xPos >= trans.x && xPos <= trans.x + trans.w && xPos >= trans.x && yPos <= trans.y + trans.h) {
                    return ent.get();
                }
            }
        }
        return nullptr;
    }

    Entity* Scene::getEntity(uint32_t Id)
    {
        for (auto& ent : mCurrentLayer->mEntities) {
            if (ent->hasComponent<Transform>()) {
                if (entt::to_integral(ent->mId) == Id)
                    return ent.get();
            }
        }
        return nullptr;
    }

    void Scene::update(float dt)
    {
        auto view = mRegistry.view<Script>();
        view.each([dt](auto& script)
            { script.update(dt); });

        auto view2 = mRegistry.view<Animation>();
        view2.each([dt](auto& anim)
            { anim.update(dt); });
    }
} // namespace Plutus
