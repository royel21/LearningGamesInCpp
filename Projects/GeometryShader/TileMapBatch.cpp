#include "TileMapBatch.h"

#include <algorithm>

#include <Graphics/Shader.h>

#include <Assets/Assets.h>
#include <Graphics/Graphic.h>
#include <Graphics/Camera2D.h>
#include <ECS/Components/TileMapComponent.h>

namespace Plutus
{
    void TileMapBatch::loadTiles(TileMapComponent* tilemap) {
        for (size_t i = 0; i < 16; i++) {
            texIndices[i] = i;
            textures[i] = nullptr;
        }

        mLayer = tilemap->mLayer;

        auto& tiles = tilemap->mTiles;

        mtiles.reserve(tiles.size() * 4);

        for (size_t i = 0; i < 16; i++) {
            textures[i] = tilemap->getTexture(i);
        }

        uint32_t offset = 0;
        for (auto t : tiles) {
            if (textures[t.texture]) {
                auto vertices = tilemap->getRect(t).getvertices();
                auto uv = textures[t.texture]->getUV(t.texcoord);

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

    void TileMapBatch::init(TileMapComponent* tilemap)
    {
        mTileMap = tilemap;
        mVAO = Graphic::createVertexArray();
        mBufferId = Graphic::createBufferArray();
        //bind the Shader position to the buffer object
        Graphic::setFAttribute(0, 2, mVertexSize);
        //bind the Shader UV "Texture coordinate" to the buffer object
        Graphic::setFAttribute(1, 2, mVertexSize, offsetof(TileVert, uvx));
        //bind the Shader Color "is a vec4 packed in a int 4 byte" to the buffer object
        Graphic::setIAttribute(2, 1, mVertexSize, offsetof(TileVert, texIndex));

        Graphic::unBind();

        loadTiles(tilemap);
    }

    void TileMapBatch::update(float dt) {
        mtiles.resize(mStaticTilesCount + mTileMap->mAnimateTiles.size() * 4);
        animVert = 0;
        int indexCount = 0;
        int tileVers = 0;

        for (size_t i = 0; i < mTileMap->mAnimateTiles.size(); i++) {
            auto& animTile = mTileMap->mAnimateTiles[i];
            animTile.currentTime += dt;

            auto anim = animTile.anim;

            if (animTile.currentTime > anim->duration) {
                animTile.frame = ++animTile.frame % anim->frames.size();
                animTile.coordIndex = anim->frames[animTile.frame];
                animTile.currentTime = 0;
            }

            auto vertices = mTileMap->getRect((float)animTile.x, (float)animTile.y).getvertices();
            auto uv = mTileMap->getTexCoord(anim->texId, animTile.coordIndex);

            mtiles[mStaticTilesCount + tileVers + 0] = { vertices[0], uv.x, uv.w, anim->texId };
            mtiles[mStaticTilesCount + tileVers + 1] = { vertices[1], uv.x, uv.y, anim->texId };
            mtiles[mStaticTilesCount + tileVers + 2] = { vertices[2], uv.z, uv.y, anim->texId };
            mtiles[mStaticTilesCount + tileVers + 3] = { vertices[3], uv.z, uv.w, anim->texId };
            indexCount++;
            animVert += 6;
            tileVers += 4;
        }

        mIbo.init(mIndexCount + indexCount);
        Graphic::uploadBufferData(mBufferId, mtiles.size() * mVertexSize, mtiles.data(), GL_STATIC_DRAW);
    }

    void TileMapBatch::draw(Shader* shader) {
        auto nShader = shader ? shader : mShader;
        if (nShader) {
            nShader->enable();
            nShader->setUniform4f("uColor", { 1, 1, 1, 1 });
            nShader->setUniformMat4("uCamera", mCamera->getCameraMatrix());
            nShader->setUniform1iv("uSampler", 16, texIndices);

            Graphic::bind(mVAO);

            mIbo.bind();
            for (size_t i = 0; i < 16; i++) {
                if (textures[i]) {
                    Graphic::bindTexture(textures[i]->mTexId, i);
                }
            }

            Graphic::drawElements(staticVert + animVert);

            mIbo.unbind();
            nShader->disable();

            Graphic::unBind();
        }
    }

    void TileMapBatch::destroy()
    {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mBufferId);
        mIbo.destroy();
        mVAO = -1;
        mBufferId = -1;
    }
}

