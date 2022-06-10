#include "Render.h"
#include "../Config.h"

#include <Graphics/GLSL.h>
#include <ECS/Components.h>
#include <Graphics/Camera2D.h>
#include <Graphics/DebugRenderer.h>

#include <Assets/Assets.h>

namespace Plutus
{
    Render::~Render() {
    }

    void Render::init(Config* config)
    {
        mConfig = config;
        reload(config);

        if (!isLoaded) {
            mDebugRender = Plutus::DebugRender::get();
            mDebugRender->init(mCamera);
            mDebugRender->setCellSize(mConfig->mProject.scene->getTileSize());
            isLoaded = true;
        }
    }

    void Render::reload(Config* config)
    {
        int w = config->mProject.vpWidth;
        int h = config->mProject.vpHeight;

        mCamera->init(w, h);
        mCamera->setPosition(config->mProject.vpPos);
        mCamera->setScale(config->mProject.zoomLevel);

        mSpriteBatch.init();
        mSpriteBatch.setCamera(mCamera);

        mFramePicker.init(w, h, true);
        mFrameBuffer.init(w, h);
        mScene = mConfig->mProject.scene.get();
    }

    void Render::resizeBuffers(const Vec2f& size) {
        mFrameBuffer.resize(size);
        mFramePicker.resize(size);
    }

    void Render::update(float dt) {
        auto viewMap = mScene->getRegistry()->view<TagComponent, TileMapComponent>();
        for (auto [ent, tag, tilemap] : viewMap.each()) {
            if (tag.Visible) {
                for (auto& tile : tilemap.mAnimateTiles)
                {
                    auto rect = mScene->getRect(tile);

                    if (mCamera->getViewPortDim().overlap(rect))
                    {
                        tile.currentTime += dt;

                        auto anim = tile.anim;

                        if (tile.currentTime > anim->duration) {
                            tile.frame = ++tile.frame % anim->frames.size();
                            tile.texcoord = anim->frames[tile.frame];
                            tile.currentTime = 0;
                        }
                    }
                }
            }
        }
    }

    void Render::draw()
    {
        if (mScene && mConfig) {
            // auto start = Time::millis();
            mFrameBuffer.setColor(mScene->mBGColor);
            prepare();
            mSpriteBatch.begin();

            mFramePicker.bind();
            mSpriteBatch.draw(BATCH_PICKING);
            mFramePicker.unBind();

            mFrameBuffer.bind();
            mSpriteBatch.draw();
            mSpriteBatch.end();

            mDebugRender->drawGrid();

            drawPhysicBodies();

            mFrameBuffer.unBind();
        }
    }

    void Render::drawFixtures(PhysicBodyComponent* pbody, TransformComponent* trans) {
        for (auto& fixture : pbody->mFixtures) {
            Vec2f pos;

            if (trans) {
                pos = trans->getPosition();//fromWorld(rbody.mBody->GetPosition());
            }

            switch (fixture.type) {
            case BoxShape: {
                Vec4f rect = { pos + fixture.offset, fixture.size.x, fixture.size.y };
                if (mCamera->getViewPortDim().overlap(rect))
                {
                    mDebugRender->submitBox(rect);
                }
                break;
            }
            case EdgeShape: {
                mDebugRender->submitLine(pos + fixture.offset, fixture.size + fixture.offset);
                break;
            }
            case CircleShape: {
                Vec4f rect = { pos.x, pos.y, fixture.radius, fixture.radius };
                if (mCamera->getViewPortDim().overlap(rect))
                {
                    mDebugRender->submitCircle(pos + fixture.offset, fixture.radius);
                }
                break;
            }
            }
        }
    }

    void Render::drawPhysicBodies()
    {
        auto view = mScene->getRegistry()->view<TagComponent, TransformComponent, RigidBodyComponent>();

        for (auto [e, tag, trans, rbody] : view.each()) {
            if (tag.Visible) {
                drawFixtures(&rbody, &trans);
            }
        }

        auto view2 = mScene->getRegistry()->view<TagComponent, PhysicBodyComponent>();
        for (auto [e, tag, pbody] : view2.each()) {
            if (tag.Visible) {
                Entity ent = { ent, mConfig->mProject.scene.get() };
                auto trans = ent.getComponent<TransformComponent>();

                drawFixtures(&pbody, trans);
            }
        }

        if (view.size_hint() || view2.size_hint()) {
            mDebugRender->end();
            mDebugRender->render();
        }
    }

    void Render::prepare()
    {
        auto viewMap = mScene->getRegistry()->view<TagComponent, TileMapComponent>();
        auto view = mScene->getRegistry()->view<TagComponent, TransformComponent, SpriteComponent>();

        /******************Resize temp buffer************************/
        auto size = view.size_hint();

        for (auto [ent, tag, map] : viewMap.each()) {
            size += map.mTiles.size() + map.mAnimateTiles.size();
        }

        if (mRenderables.size() != size + mTotalTemp) {
            mRenderables.resize(size + mTotalTemp);
        }

        /******************************************/

        int i = mTotalTemp;
        for (auto [ent, tag, tilemap] : viewMap.each())
        {
            auto texIndex = -1;
            Texture* tex = nullptr;

            if (tag.Visible && tilemap.mTiles.size())
            {
                const int w = mScene->mTileWidth;
                const int h = mScene->mTileHeight;

                for (auto& tile : tilemap.mTiles)
                {
                    auto rect = mScene->getRect(tile);
                    if (mCamera->getViewPortDim().overlap(rect))
                    {
                        if (texIndex != tile.texture) {
                            tex = tilemap.getTexture(tile.texture);
                            texIndex = tile.texture;
                        }

                        if (tex) {
                            mRenderables[i++] = {
                                tex, rect, tex->getUV(tile.texcoord), {},
                                tile.rotate, tile.flipX, tile.flipY, (int)entt::to_integral(ent),
                                tilemap.mLayer, false
                            };
                        }
                    }
                }

                for (auto& tile : tilemap.mAnimateTiles)
                {
                    auto rect = mScene->getRect(tile);

                    if (mCamera->getViewPortDim().overlap(rect))
                    {
                        auto texIndex = tile.anim->texId;

                        if (texIndex != texIndex) {
                            tex = tilemap.getTexture(texIndex);
                            texIndex = texIndex;
                        }
                        if (tex) {
                            mRenderables[i++] = {
                                tex, rect, tex->getUV(tile.texcoord), {},
                                0, false, false, (int)entt::to_integral(ent),
                                tilemap.mLayer, false
                            };
                        }
                    }
                }
            }
        }

        for (auto [ent, tag, trans, sprite] : view.each())
        {
            if (!tag.Visible) continue;

            auto rect = trans.getRect();
            if (mCamera->getViewPortDim().overlap(rect))
            {
                auto tex = AssetManager::get()->getAsset<Texture>(sprite.mTextureId);

                mRenderables[i++] = {
                    tex, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent),
                    trans.layer, trans.sortY
                };
            }

        }

        mRenderables.resize(i);
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());

        mSpriteBatch.submit(mRenderables);
    }
}