#include "Render.h"
#include "../Config.h"

#include <Graphics/GLSL.h>
#include <ECS/Components.h>
#include <Graphics/DebugRenderer.h>

#include <Assets/Assets.h>

#include <Time/Timer.h>

namespace Plutus
{
    Render::~Render() {
        mShader.destroy();
    }

    void Render::init(Config* config)
    {
        mConfig = config;
        reload(config);

        if (!isLoaded) {
            mShader.init(GLSL::vertexShader, GLSL::fragShader);
            mDebugRender = Plutus::DebugRender::get();
            mDebugRender->init(&mCamera);
            mDebugRender->setCellSize({ mConfig->tileWidth, mConfig->tileHeight });
            isLoaded = true;
        }
    }

    void Render::reload(Config* config)
    {
        int w = config->mProject.vpWidth;
        int h = config->mProject.vpHeight;

        mCamera.init(w, h);
        mCamera.setPosition(config->mProject.vpPos);
        mCamera.setScale(config->mProject.zoomLevel);

        mSpriteBatch.init();
        mSpriteBatch.setShader(&mShader);
        mSpriteBatch.setCamera(&mCamera);

        mFramePicker.init(w, h, true);
        mFrameBuffer.init(w, h);
        mScene = mConfig->mProject.scene.get();
    }

    void Render::resizeBuffers(const Vec2f& size) {
        mFrameBuffer.resize(size);
        mFramePicker.resize(size);
    }

    void Render::draw()
    {
        mCamera.update();
        if (mScene && mConfig) {
            // auto start = Timer::millis();
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
                if (mCamera.isBoxInView(rect, 200))
                {
                    mDebugRender->drawBox(rect);
                }
                break;
            }
            case EdgeShape: {
                mDebugRender->drawLine(pos + fixture.offset, fixture.size + fixture.offset);
                break;
            }
            case CircleShape: {
                Vec4f rect = { pos.x, pos.y, fixture.radius, fixture.radius };
                if (mCamera.isBoxInView(rect, 200))
                {
                    mDebugRender->drawCircle(pos + fixture.offset, fixture.radius);
                }
                break;
            }
            }
        }
    }

    void Render::drawPhysicBodies()
    {
        auto view = mScene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [e, trans, rbody] : view.each()) {
            drawFixtures(&rbody, &trans);
        }

        auto view2 = mScene->getRegistry()->view<PhysicBodyComponent>();
        for (auto [e, pbody] : view2.each()) {
            Entity ent = { ent, mConfig->mProject.scene.get() };
            auto trans = ent.getComponent<TransformComponent>();

            drawFixtures(&pbody, trans);
        }

        if (view.size_hint()) {
            mDebugRender->end();
            mDebugRender->render();
        }
    }

    void Render::prepare()
    {
        auto viewMap = mScene->getRegistry()->view<TileMapComponent>();
        auto view = mScene->getRegistry()->view<TransformComponent, SpriteComponent>();

        /******************Resize temp buffer************************/
        auto size = view.size_hint();

        for (auto [ent, map] : viewMap.each()) {
            size += map.mTiles.size();
        }

        if (mRenderables.size() != size + mTotalTemp) {
            mRenderables.resize(size + mTotalTemp);
        }

        /******************************************/

        int i = mTotalTemp;
        for (auto ent : viewMap)
        {
            auto [tilemap] = viewMap.get(ent);
            if (tilemap.mTiles.size())
            {
                const int w = tilemap.mTileWidth;
                const int h = tilemap.mTileHeight;

                for (auto& tile : tilemap.mTiles)
                {
                    auto rect = tile.getRect();
                    if (mCamera.isBoxInView(rect, 200))
                    {
                        auto texIndex = -1;
                        Texture* tex = nullptr;
                        uint32_t texId;

                        if (texIndex != tile.texture) {
                            tex = tilemap.getTexture(tile.texture);
                            texId = tex ? tex->mTexId : -1;
                        }

                        if (tex) {
                            mRenderables[i++] = { texId, rect, tex->getUV(tile.texcoord), {}, tile.rotate, tile.flipX, tile.flipY, (int)entt::to_integral(ent), tilemap.mLayer, false };
                        }
                    }
                }
            }
        }

        for (auto ent : view)
        {
            auto [trans, sprite] = view.get(ent);
            auto rect = trans.getRect();
            if (mCamera.isBoxInView(rect, 200))
            {
                auto tex = AssetManager::get()->getAsset<Texture>(sprite.mTextureId);

                auto texId = tex ? tex->mTexId : -1;

                mRenderables[i++] = { texId, rect, sprite.mUVCoord, sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
            }

        }

        mRenderables.resize(i);
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());

        mSpriteBatch.submit(mRenderables);
    }
}