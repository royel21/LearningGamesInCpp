#include "Render.h"
#include "../Config.h"

#include <Graphics/GLSL.h>
#include <ECS/Components.h>
#include <Graphics/DebugRenderer.h>

namespace Plutus
{
    Render& Render::get()
    {
        static Render render;
        return render;
    }

    void Render::Init()
    {
        auto& config = Config::get();
        int w = config.mProject->vpWidth;
        int h = config.mProject->vpHeight;

        mCamera.init(w, h);
        mCamera.setPosition(config.vpPos);
        mCamera.setScale(config.vpZoom);

        mShader.CreateProgWithShader(GLSL::vertexShader, GLSL::fragShader);

        mSpriteBatch.init();
        mFramePicker.init(w, h, true);
        mFrameBuffer.init(w, h);

        mDebugRender = Plutus::DebugRender::get();
        mDebugRender->init(&mCamera);
        mDebugRender->setCellSize({ 64,64 });
    }

    void Render::draw()
    {
        prepare();
        mSpriteBatch.begin(&mShader, &mCamera);

        mFramePicker.bind();
        mSpriteBatch.draw(true);
        mFramePicker.unBind();

        mFrameBuffer.bind();
        mFrameBuffer.setColor(Config::get().vpColor);
        mSpriteBatch.draw();
        mSpriteBatch.end();

        mDebugRender->drawGrid();
        mFrameBuffer.unBind();
    }

    void Render::prepare()
    {
        auto viewMap = Config::get().mProject->mScene->getRegistry()->view<TileMap>();
        auto view = Config::get().mProject->mScene->getRegistry()->view<Transform, Sprite>();

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
                    glm::vec4 rect = tile.getRect();
                    if (mCamera.isBoxInView(rect, 200))
                    {
                        auto tex = tilemap.getTexture(tile.texture);

                        mRenderables[i++] = { tex->texId, rect, tex->getUV(tile.texcoord),
                            { tile.color }, tile.rotate, tile.flipX, tile.flipY, (int)entt::to_integral(ent), tilemap.mLayer, false };
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
                mRenderables[i++] = { sprite.getTexId(), rect, sprite.getUV(), sprite.mColor,
                    trans.r, sprite.mFlipX, sprite.mFlipY, (int)entt::to_integral(ent), trans.layer, trans.sortY };
            }

        }

        mRenderables.resize(i);
        // sort by layer, y position, texture
        std::sort(mRenderables.begin(), mRenderables.end());

        mSpriteBatch.submit(mRenderables);
    }
}