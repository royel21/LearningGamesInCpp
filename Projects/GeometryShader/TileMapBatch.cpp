#include "TileMapBatch.h"

#include <algorithm>

#include <Graphics/Shader.h>

#include <Core/Project.h>
#include <Assets/Assets.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/TileMapComponent.h>

#include <Log/Logger.h>

namespace Plutus
{
    void TileMapBatch::loadTiles(TileMapComponent* tilemap) {
        for (size_t i = 0; i < 16; i++) {
            texIndices[i] = i;
            mTextures[i] = nullptr;
        }

        mLayer = tilemap->mLayer;

        mtiles.reserve(tilemap->mTiles.size() * 4);

        for (size_t i = 0; i < 16; i++) {
            mTextures[i] = tilemap->getTexture(i);
        }

        uint32_t offset = 0;
        for (auto t : tilemap->mTiles) {
            if (mTextures[t.texture]) {
                auto vertices = mProject->scene->getRect(t).getvertices();
                auto uv = mTextures[t.texture]->getUV(t.texcoord);

                mtiles.emplace_back(vertices[0], uv.x, uv.w, t.texture);
                mtiles.emplace_back(vertices[1], uv.x, uv.y, t.texture);
                mtiles.emplace_back(vertices[2], uv.z, uv.y, t.texture);
                mtiles.emplace_back(vertices[3], uv.z, uv.w, t.texture);

                staticVert += 6;
                mStaticTilesCount += 4;
                mIndexCount++;
            }
        }
    }

    void TileMapBatch::init(TileMapComponent* tilemap, Project* proj)
    {
        mProject = proj;
        mTileMap = tilemap;
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        mIboId = Graphic::createElementBuffer();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, mVertexSize, offsetof(TileVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(2, 1, mVertexSize, offsetof(TileVert, texIndex));

        Graphic::uploadIndices(mIboId, tilemap->mTiles.size() + mTileMap->mAnimateTiles.size());
        Graphic::unBind();

        loadTiles(tilemap);
    }

    void TileMapBatch::update(float dt) {
        uint32_t total = mStaticTilesCount + mTileMap->mAnimateTiles.size() * 4;
        if (mtiles.size() < total) {
            mtiles.resize(total);
        }
        animVert = 0;
        int tileVers = 0;

        for (size_t i = 0; i < mTileMap->mAnimateTiles.size(); i++) {
            auto& animTile = mTileMap->mAnimateTiles[i];
            auto rect = mProject->scene->getRect(animTile);

            animTile.currentTime += dt;

            auto anim = animTile.anim;

            if (animTile.currentTime > anim->duration) {
                animTile.frame = ++animTile.frame % anim->frames.size();
                animTile.texcoord = anim->frames[animTile.frame];
                animTile.currentTime = 0;
            }

            auto vertices = rect.getvertices();
            auto uv = mTileMap->getTexCoord(anim->texId, animTile.texcoord);

            mtiles[mStaticTilesCount + tileVers++] = { vertices[0], uv.x, uv.w, anim->texId };
            mtiles[mStaticTilesCount + tileVers++] = { vertices[1], uv.x, uv.y, anim->texId };
            mtiles[mStaticTilesCount + tileVers++] = { vertices[2], uv.z, uv.y, anim->texId };
            mtiles[mStaticTilesCount + tileVers++] = { vertices[3], uv.z, uv.w, anim->texId };
            animVert += 6;
        }
    }

    void TileMapBatch::draw(Shader* shader) {
        auto nShader = shader ? shader : mShader;
        if (nShader) {
            nShader->enable();
            nShader->setUniform4f("uColor", { 1, 1, 1, 1 });
            nShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());
            nShader->setUniform1iv("uSampler", 16, texIndices);

            Graphic::enableBlend();

            Graphic::bind(mVAO, mIboId);

            Graphic::uploadBufferData(mBufferId, mtiles.size() * mVertexSize, mtiles.data(), GL_DYNAMIC_DRAW);

            for (size_t i = 0; i < 16; i++) {
                if (mTextures[i]) {
                    Graphic::bindTexture(mTextures[i]->mTexId, i);
                }
            }

            Graphic::drawElements(staticVert + animVert);

            Graphic::unBind();
            nShader->disable();
        }
    }

    void TileMapBatch::destroy()
    {
        Graphic::destroy(&mVAO, &mBufferId, &mIboId);
    }
}

