#include "TileMapPanel.h"

#include <vector>
#include <imgui.h>
#include <ECS/Components/TileMapComponent.h>

#include <Math/Vectors.h>
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include "ComponentUtil.h"

#include <stdio.h>

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    vec2i getCoords() {
        auto pos = Config::get().mMouseCoords;
        return DebugRender::get()->getSquareCoords({ pos.x, pos.y });
    }

    TileMapPanel::TileMapPanel()
    {
        Input::get()->addEventListener(this);
    }

    void TileMapPanel::DrawTileMapComponet()
    {
        if (CollapseComponent<TileMapComponent>("TileMap##tilemap-comp", 5))
        {
            static bool addTexture = false;
            mIsOpen = true;
            mTileMap = Config::get().mProject->mEnt.getComponent<TileMapComponent>();
            ImGui::BeginUIGroup();
            {
                ImGui::BeginCol("Tile Width");
                ImGui::InputInt("#tm-w", &mTileMap->mTileWidth);
                ImGui::BeginCol("Tile Heigth");
                ImGui::InputInt("#tm-h", &mTileMap->mTileHeight);
                ImGui::BeginCol("Layer");
                ImGui::InputInt("#tm-l", &mTileMap->mLayer);
            }
            ImGui::EndUIGroup();
            ImGui::Separator();
            //Texture Combobox
            ImGui::Text("Textures");
            ImGui::SameLine();
            ImGui::PushItemWidth(120);
            if (ImGui::ComboBox("##mttexture", mTileMap->mTextures, mCurrentTexture)) {
                mTempTiles.clear();
                if (mCurrentTile)
                    mCurrentTile->texcoord = 0;
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();
            //Add, Remove Buttons
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " ##tm-add-tex")) {
                addTexture = true;
            }
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_TRASH " ##tm-rm-tex")) {
                mTempTiles.clear();
                mTileMap->removeTexture(mCurrentTexture);
                if (mCurrentTile)
                    mCurrentTile->texcoord = 0;
            }
            ImGui::PopStyleVar();
            ImGui::Separator();

            AddTexureDialog(addTexture);

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
                    ImGui::DrawTexture(mTileMap->getTexture(mCurrentTexture), 0, 0, scale, &mTempTiles);
                }
            }

            if (mCurrentTile != nullptr && mMode == MODE_EDIT)
            {
                ImGui::Text("Tile Props");
                ImGui::Separator();
                if (ImGui::BeginUIGroup()) {
                    ImGui::BeginCol("X");
                    ImGui::InputInt("##ctile-x", &mCurrentTile->x, 1);

                    ImGui::BeginCol("Y");
                    ImGui::InputInt("##ctile-y", &mCurrentTile->y, 1);

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
                        auto size = mTileMap->getTexture(mCurrentTile->texture)->uvs.size();
                        mCurrentTile->texcoord = LIMIT(texcoord, 0, (int)size - 1);
                    }
                    ImGui::BeginCol("Textures");
                    if (ImGui::ComboBox("##mttexture", mTileMap->mTextures, mCurrentTile->texture)) {
                        mCurrentTile->texcoord = 0;
                    }
                    ImGui::EndUIGroup();
                }
            }
            if (mMode == MODE_PLACE && Config::get().isHover) {
                renderTemp();
            }
            else {
                Render::get().mTotalTemp = 0;
            }
        }
        else {
            mIsOpen = false;
        }

    }
    void TileMapPanel::processMode() {
        if (mIsOpen && Config::get().isHover && Input::get()->onKeyDown("MouseLeft")) {
            switch (mMode)
            {
            case MODE_PLACE:
                createTiles();
                break;
            case MODE_EDIT:
                mCurrentTile = mTileMap->getTile(getCoords());
                break;
            default:
                if (mTileMap->removeTile(getCoords())) {
                    mCurrentTile = nullptr;
                }
                break;
            }
        }
    }
    void TileMapPanel::onKeyDown(const std::string& key)
    {
        processMode();
    }

    void TileMapPanel::onMouseMove(float x, float y)
    {
        processMode();
    }

    void TileMapPanel::createTiles() {
        auto coords = getCoords();
        auto& tiles = mTileMap->mTiles;
        for (auto tile : mTempTiles)
        {
            int x = (int)coords.x + tile.x;
            int y = (int)coords.y - tile.y;

            int index = mTileMap->getIndex({ x, y });
            if (index == -1)
            {
                tiles.push_back(Tile(x, y, tile.z, mCurrentTexture, mRotation));
                tiles.back().setParent(mTileMap);
            }
            else
            {
                mTileMap->mTiles[index].texcoord = tile.z;
                mTileMap->mTiles[index].texture = mCurrentTexture;
            }
        }
    }

    void TileMapPanel::renderTemp()
    {
        auto gridCoords = getCoords();
        int w = mTileMap->mTileWidth;
        int h = mTileMap->mTileHeight;

        auto tex = mTileMap->getTexture(mCurrentTexture);

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
            if (tex)
                renderables[i++] = { tex->texId, { x, y, w, h }, tex->getUV(tile.z), {}, mRotation, false, false, -1, 99, false };
        }
    }

    void TileMapPanel::AddTexureDialog(bool& show) {
        if (show) {
            static float scale = 1;
            auto textures = AssetManager::get()->mTextures.getItems();
            static std::string current = textures.begin()->second.name;
            int index = (int)mTileMap->mTextures.size();

            ImGui::BeginDialog("Texture Modal");
            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::BeginCol("Textures");
                ImGui::ComboBox("##mt-add-tex", textures, current);
                ImGui::BeginCol("Scale");
                ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
                ImGui::EndUIGroup();
            }

            ImGui::DrawTexture(&textures[current], 400, 350, scale);
            ImGui::Separator();
            if (ImGui::Button("Add Texture##mt-modal"))
            {
                int i = 0;
                auto it = std::find_if(mTileMap->mTextures.begin(), mTileMap->mTextures.end(), [&](auto&& val)->bool {
                    i = std::max(i, val.first);
                    return val.second.compare(current) == 0;
                    });

                if (it == mTileMap->mTextures.end()) {
                    mTileMap->mTextures[i + 1] = current;
                    mCurrentTexture = i + 1;
                }
                show = false;
            }
            ImGui::EndDialog(show);
        }
    }
}