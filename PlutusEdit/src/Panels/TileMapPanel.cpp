#include "TileMapPanel.h"

#include <vector>
#include <imgui.h>
#include <ECS/Components/TileMap.h>

#include "ComponentUtil.h"

#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"
#include "../Config.h"

#include <glm/glm.hpp>
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <stdio.h>

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    glm::vec2 getCoords() {
        auto pos = Config::get().mProject->mVpState.mouseCoords;
        return DebugRender::get()->getSquareCoords({ pos.x, pos.y });
    }

    TileMapPanel::TileMapPanel()
    {
        Input::get()->addEventListener(this);
    }

    void TileMapPanel::DrawTileMapComponet()
    {
        if (CollapseComponent<TileMap>("TileMap##tilemap-comp", 5))
        {
            mIsOpen = true;
            mTileMap = Config::get().mProject->mEnt.getComponent<TileMap>();
            ImGui::BeginUIGroup();
            {
                ImGui::BeginCol("Tile Width");
                ImGui::InputInt("#tm-w", &mTileMap->mTileWidth);
                ImGui::BeginCol("Tile Heigth");
                ImGui::InputInt("#tm-w", &mTileMap->mTileHeight);
                ImGui::BeginCol("Layer");
                ImGui::InputInt("#tm-w", &mTileMap->mLayer);
            }
            ImGui::EndUIGroup();
            ImGui::Separator();

            if (mTileMap->mTextures.size())
            {
                ImGui::RadioButton("Place", &mMode, MODE_PLACE);
                ImGui::SameLine();
                ImGui::RadioButton("Edit", &mMode, MODE_EDIT);
                ImGui::SameLine();
                ImGui::RadioButton("Remove", &mMode, MODE_REMOVE);
                ImGui::Separator();
                if (mMode == MODE_PLACE)
                {
                    static float scale = 1.0f;
                    ImGui::BeginUIGroup();
                    {
                        ImGui::BeginCol("Rotation");
                        if (ImGui::InputFloat("#tm-r", &mRotation, 45.0f, 90.0f, "%0.0f")) {
                            mRotation = LIMIT(mRotation, 0.0f, 360.0f);
                        }
                        ImGui::BeginCol("Scale");
                        ImGui::DragFloat("##sc-tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
                    }
                    ImGui::EndUIGroup();
                    ImGui::DrawTexture(mTileMap->mTextures[mCurrentTexture], 0, 0, scale, &mTempTiles);
                }
            }


            if (mCurrentTile != nullptr && mMode == MODE_EDIT)
            {
                ImGui::Text("Tile Props");
                ImGui::Separator();
                if (ImGui::BeginUIGroup()) {
                    ImGui::BeginCol("X");
                    ImGui::InputInt("##ctile-x", &mCurrentTile->x, mTileMap->mTileWidth);

                    ImGui::BeginCol("Y");
                    ImGui::InputInt("##ctile-x", &mCurrentTile->y, mTileMap->mTileHeight);

                    ImGui::BeginCol("Rotation");
                    float rotation = mCurrentTile->rotate;
                    if (ImGui::InputFloat("##ctile-r", &rotation, 45.0f, 90.0f, "%0.0f"))
                    {
                        mCurrentTile->rotate = LIMIT(rotation, 0.0f, 360.0f);
                    }

                    ImGui::BeginCol("Flip X");
                    ImGui::Checkbox("##cTile-flix", &mCurrentTile->flipX);

                    ImGui::BeginCol("Flip Y");
                    ImGui::Checkbox("##cTile-lipy", &mCurrentTile->flipY);

                    ImGui::BeginCol("Texture");
                    int texcoord = mCurrentTile->texcoord;
                    if (ImGui::InputInt("##ctile-texture", &texcoord, 1))
                    {
                        auto size = mTileMap->mTextures[mCurrentTexture]->uvs.size();
                        mCurrentTile->texcoord = LIMIT(texcoord, 0, (int)size - 1);
                    }
                    ImGui::EndUIGroup();
                }
            }
            if (mMode == MODE_PLACE && Config::get().mProject->mVpState.isHover) {
                renderTemp();
            }
        }
        else {
            mIsOpen = false;
        }

    }

    void TileMapPanel::onKeyDown(const std::string& key)
    {
        if (mIsOpen) {
            bool isHover = Config::get().mProject->mVpState.isHover;
            if (isHover && key.compare("MouseLeft") == 0 && mMode == MODE_EDIT) {
                selectTile();
            }
        }
    }

    void TileMapPanel::onMouseMove(float x, float y)
    {
        if (mIsOpen) {
            bool isHover = Config::get().mProject->mVpState.isHover;
            if (isHover) {

                if (Input::get()->onKeyDown("MouseLeft")) {
                    switch (mMode)
                    {
                    case MODE_PLACE:
                        break;
                    case MODE_EDIT:
                        selectTile();
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    void TileMapPanel::selectTile()
    {
        mCurrentTile = mTileMap->getTile(getCoords());
    }

    void TileMapPanel::renderTemp()
    {
        auto gridCoords = getCoords();
        int w = mTileMap->mTileWidth;
        int h = mTileMap->mTileHeight;

        auto& tex = mTileMap->mTextures[mCurrentTexture];

        std::vector<Renderable>& renderables = Render::get().mRenderables;
        if (renderables.size() < mTempTiles.size()) {
            renderables.resize(mTempTiles.size());
        }
        Render::get().mTotalTemp = (int)mTempTiles.size();

        int i = 0;
        for (auto tile : mTempTiles)
        {
            int x = ((int)gridCoords.x * w) + (tile.x * w);
            int y = ((int)gridCoords.y * h) - (tile.y * h);

            renderables[i++] = { tex->texId, { x, y, w, h }, tex->getUV(tile.z), {}, mRotation, false, false, -1, 99, false };
        }
    }
}