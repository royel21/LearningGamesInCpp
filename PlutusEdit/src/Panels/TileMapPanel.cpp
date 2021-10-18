#include "TileMapPanel.h"

#include <vector>
#include <imgui.h>
#include <ECS/Components/TileMap.h>

#include "ComponentUtil.h"

#include "../Helpers/Render.h"
#include "../Helpers/ImGuiEx.h"
#include "../Config.h"

#include <glm/glm.hpp>
#include <Graphics/DebugRenderer.h>
#include <Input/Input.h>

#include <stdio.h>

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
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

            bool isHover = Config::get().mProject->mVpState.isHover;
            if (isHover && mTempTiles.size() && mMode == MODE_PLACE && mTileMap->mTextures.size())
            {

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
            if (isHover && Input::get()->onKeyDown("MouseLeft") && mMode == MODE_EDIT) {
                selectTile();
            }
        }
    }

    void TileMapPanel::selectTile()
    {
        auto pos = Config::get().mProject->mVpState.mouseCoords;
        auto mCoords = DebugRender::get()->getSquareCoords({ pos.x, pos.y });
        mCurrentTile = mTileMap->getTile(mCoords);
    }
}