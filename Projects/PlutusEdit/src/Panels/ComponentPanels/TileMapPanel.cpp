#include "TileMapPanel.h"

#include <vector>
#include <imgui.h>
#include <ECS/Components/TileMapComponent.h>

#include <Math/Vectors.h>
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include "ComponentUtil.h"

#include <stdio.h>

#include <Assets/Assets.h> 

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    vec2i getCoords(Config* config) {
        auto pos = config->mMouseCoords;
        return DebugRender::get()->getSquareCoords({ pos.x, pos.y });
    }

    TileMapPanel::TileMapPanel()
    {
        Input::get()->addEventListener(this);
    }

    void TileMapPanel::draw(Config* config)
    {
        mConfig = config;
        if (CollapseComponent<TileMapComponent>("TileMap##tilemap-comp", 4, mConfig))
        {
            static bool addTexture = false;
            mIsOpen = true;
            mTileMap = mConfig->mProject.mEnt.getComponent<TileMapComponent>();

            for (int i = 0; i < 16; i++) {
                if (!mTileMap->mTextures[i].empty()) {
                    mCurrentTexture = i;
                    break;
                }
            }

            float textWidth = ImGui::GetContentRegionAvailWidth() * 0.35f;
            ImGui::Row("Width", textWidth);
            ImGui::InputInt("##tm-w", &mTileMap->mWidth);
            ImGui::Row("Height", textWidth);
            ImGui::InputInt("##tm-h", &mTileMap->mHeight);
            ImGui::Row("Tile Width", textWidth);
            ImGui::InputInt("##tm-tw", &mTileMap->mTileWidth);
            ImGui::Row("Tile Heigth", textWidth);
            ImGui::InputInt("##tm-th", &mTileMap->mTileHeight);
            ImGui::Row("Layer", textWidth);
            ImGui::InputInt("##tm-l", &mTileMap->mLayer);

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
            ImGui::Text("Tiles Count: %i", mTileMap->mTiles.size());
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
                    ImGui::Row("Rotation", textWidth);
                    if (ImGui::InputFloat("##tm-r", &mRotation, 45.0f, 90.0f, "%0.0f")) {
                        mRotation = LIMIT(mRotation, 0.0f, 360.0f);
                    }

                    ImGui::Row("Scale", textWidth);
                    ImGui::DragFloat("##sc-tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
                    ImGui::DrawTexture(mTileMap->getTexture(mCurrentTexture), 0, 0, scale, &mTempTiles);
                }
            }

            if (mCurrentTile != nullptr && mMode == MODE_EDIT)
            {
                ImGui::Text("Tile Props");
                ImGui::Separator();
                {
                    ImGui::Row("X", textWidth);
                    ImGui::InputInt("##ctile-x", &mCurrentTile->x, 1);

                    ImGui::Row("Y", textWidth);
                    ImGui::InputInt("##ctile-y", &mCurrentTile->y, 1);

                    ImGui::Row("Rotation", textWidth);
                    float rotation = mCurrentTile->rotate;
                    if (ImGui::InputFloat("##ctile-r", &rotation, 45.0f, 90.0f, "%0.0f"))
                    {
                        mCurrentTile->rotate = LIMIT(rotation, 0.0f, 360.0f);
                    }

                    ImGui::Row("Flip X", textWidth);
                    ImGui::Checkbox("##cTile-flix", &mCurrentTile->flipX);

                    ImGui::Row("Flip Y", textWidth);
                    ImGui::Checkbox("##cTile-lipy", &mCurrentTile->flipY);

                    ImGui::Row("Texture", textWidth);
                    int texcoord = mCurrentTile->texcoord;
                    if (ImGui::InputInt("##ctile-texture", &texcoord, 1))
                    {
                        auto size = mTileMap->getTexture(mCurrentTile->texture)->uvs.size();
                        mCurrentTile->texcoord = LIMIT(texcoord, 0, (int)size - 1);
                    }
                    ImGui::Row("Textures", textWidth);
                    if (ImGui::ComboBox("##mttexture", mTileMap->mTextures, mCurrentTile->texture)) {
                        mCurrentTile->texcoord = 0;
                    }
                }
            }
            if (mMode == MODE_PLACE && mConfig->isHover) {
                renderTemp();
            }
            else {
                mConfig->mRender->mTotalTemp = 0;
            }
        }
        else {
            mIsOpen = false;
        }

    }

    void TileMapPanel::processMode() {
        if (mIsOpen && mConfig->isHover && Input::get()->onKeyDown("MouseLeft")) {
            switch (mMode)
            {
            case MODE_PLACE:
                createTiles();
                break;
            case MODE_EDIT:
                mCurrentTile = mTileMap->getTile(getCoords(mConfig));
                break;
            default:
                if (mTileMap->removeTile(getCoords(mConfig))) {
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
        auto coords = getCoords(mConfig);
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
        auto gridCoords = getCoords(mConfig);
        int w = mTileMap->mTileWidth;
        int h = mTileMap->mTileHeight;

        auto tex = mTileMap->getTexture(mCurrentTexture);

        std::vector<Renderable>& renderables = mConfig->mRender->mRenderables;
        if (renderables.size() < mTempTiles.size()) {
            renderables.resize(mTempTiles.size());
        }
        mConfig->mRender->mTotalTemp = (int)mTempTiles.size();

        int i = 0;
        for (auto tile : mTempTiles)
        {
            int x = ((int)gridCoords.x * w) + (tile.x * w);
            int y = ((int)gridCoords.y * h) - (tile.y * h);
            if (tex)
                renderables[i++] = { tex->mTexId, { x, y, w, h }, tex->getUV(tile.z), {}, mRotation, false, false, -1, 99, false };
        }
    }

    void TileMapPanel::AddTexureDialog(bool& show) {
        if (show) {
            static float scale = 1;
            auto& textures = AssetManager::get()->getAssets<Texture>();
            static std::string current = textures.begin()->first;
            int index = (int)mTileMap->mTextures.size();

            float width = ImGui::GetContentRegionAvailWidth() * 0.3f;

            ImGui::BeginDialog("Texture Modal");

            ImGui::Row("Textures", width);
            ImGui::ComboBox("##mt-add-tex", textures, current);
            ImGui::Row("Scale", width);
            ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");

            auto texture = static_cast<Texture*>(textures[current]);
            ImGui::DrawTexture(texture, 400, 350, scale);
            ImGui::Separator();
            if (ImGui::Button("Add Texture##mt-modal"))
            {
                mTileMap->addTexture(current);
                show = false;
            }
            ImGui::EndDialog(show);
        }
    }
}