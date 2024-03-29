#include "TileMapPanel.h"

#include <vector>
#include <imgui.h>
#include <ECS/Components/TileMapComponent.h>

#include <Math/Vectors.h>
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include "ComponentUtil.h"

#include <Assets/Assets.h> 

#include <Log/Logger.h>

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    Vec2i getCoords(Config* config) {
        auto pos = config->mMouseCoords;
        return DebugRender::get()->getSquareCoords({ pos.x, pos.y });
    }

    TileMapPanel::TileMapPanel()
    {
        Input.addEventListener(this);
    }

    void TileMapPanel::draw(Config* config)
    {
        mConfig = config;
        static bool loadTex = true;

        if (CollapseComponent<TileMapComponent>("TileMap##tilemap-comp", 4, mConfig))
        {
            static bool addTexture = false;
            mIsOpen = true;
            mTileMap = mConfig->mProject.mEnt.getComponent<TileMapComponent>();
            if (loadTex) {
                for (int i = 0; i < 16; i++) {
                    if (!mTileMap->mTextures[i].empty()) {
                        mCurrentTexture = i;
                        break;
                    }
                }
                loadTex = false;
            }

            float textWidth = ImGui::GetContentRegionAvailWidth() * 0.35f;
            ImGui::Row("Width", textWidth);
            ImGui::InputInt("##tm-w", &mTileMap->mWidth);
            ImGui::Row("Height", textWidth);
            ImGui::InputInt("##tm-h", &mTileMap->mHeight);
            ImGui::Row("Layer", textWidth);
            ImGui::InputInt("##tm-l", &mTileMap->mLayer);

            ImGui::Separator();
            //Texture Combobox
            ImGui::Text("Textures");
            ImGui::SameLine();
            //Add, Remove Buttons
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " ##tm-add-tex")) {
                addTexture = true;
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 20);
            if (ImGui::ComboBox("##mttexture", mTileMap->mTextures, mCurrentTexture)) {
                mTempTiles.clear();
                if (mCurrentTile)
                    mCurrentTile->texcoord = 0;
            }
            ImGui::SameLine();
            if (ImGui::TransparentButton(ICON_FA_TRASH " ##tm-rm-tex", true, { 1,0,0,1 })) {
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
            }

            switch (mMode) {
            case MODE_PLACE:
            {
                placeMode(textWidth);
                break;
            }
            case MODE_EDIT: {
                editMode(textWidth);
                break;
            }
            }
        }
        else {
            mIsOpen = false;
            loadTex = true;
        }

    }

    void TileMapPanel::processMode() {
        if (mIsOpen && mConfig->isHover) {
            auto coord = getCoords(mConfig);
            if (Input.onKeyPressed("MouseLeft") && mMode == MODE_EDIT) {
                mCurrentTile = mTileMap->getTile(coord);
            }

            if (Input.onKeyDown("MouseLeft")) {
                switch (mMode)
                {
                case MODE_PLACE:
                    createTiles();
                    break;
                case MODE_EDIT: {
                    if (mCurrentTile) {
                        mCurrentTile->x = coord.x;
                        mCurrentTile->y = coord.y;
                    }
                    break;
                }
                default:
                    if (mTileMap->removeTile(coord)) {
                        mCurrentTile = nullptr;
                    }
                    break;
                }
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
        int w = mConfig->mProject.scene->mTileWidth;
        int h = mConfig->mProject.scene->mTileHeight;

        auto tex = mTileMap->getTexture(mCurrentTexture);

        std::vector<Renderable>& renderables = mConfig->mRender.mRenderables;
        if (renderables.size() < mTempTiles.size()) {
            renderables.resize(mTempTiles.size());
        }
        mConfig->mRender.mTotalTemp = (int)mTempTiles.size();

        int i = 0;
        for (auto tile : mTempTiles)
        {
            int x = ((int)gridCoords.x * w) + (tile.x * w);
            int y = ((int)gridCoords.y * h) - (tile.y * h);
            if (tex)
                renderables[i++] = { tex, { x, y, w, h }, tex->getUV(tile.z), {}, mRotation, false, false, -1, 99, false };
        }
    }

    void TileMapPanel::AddTexureDialog(bool& show) {
        if (show) {
            static float scale = 1;
            auto& textures = AssetManager::get()->getAssets<Texture>();
            static std::string current = textures.begin()->first;
            int index = (int)mTileMap->mTextures.size();
            ImGui::SetNextWindowSize({ 400, 0 });

            float width = ImGui::GetContentRegionAvailWidth() * 0.3f;
            ImGui::BeginDialog("Texture Modal");

            ImGui::Row("Textures", width);
            ImGui::ComboBox("##mt-add-tex", textures, current);
            ImGui::Row("Scale", width);
            ImGui::DragFloat("##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");

            auto texture = static_cast<Texture*>(textures[current]);
            ImGui::DrawTexture(texture, 400, 350, scale);
            ImGui::EndDialog(show, [&](bool save) {
                if (save) {
                    mTileMap->addTexture(current);
                    show = false;
                }
                });
        }
    }

    void TileMapPanel::editMode(float width)
    {
        if (!mCurrentTile) return;

        ImGui::Text("Tile Props");
        ImGui::Separator();
        {
            ImGui::Row("X", width);
            ImGui::InputInt("##ctile-x", &mCurrentTile->x, 1);

            ImGui::Row("Y", width);
            ImGui::InputInt("##ctile-y", &mCurrentTile->y, 1);

            ImGui::Row("Rotation", width);
            float rotation = mCurrentTile->rotate;
            if (ImGui::InputFloat("##ctile-r", &rotation, 45.0f, 90.0f, "%0.0f"))
            {
                mCurrentTile->rotate = LIMIT(rotation, 0.0f, 360.0f);
            }

            ImGui::Row("Flip X", width);
            ImGui::Checkbox("##cTile-flix", &mCurrentTile->flipX);

            ImGui::Row("Flip Y", width);
            ImGui::Checkbox("##cTile-lipy", &mCurrentTile->flipY);

            ImGui::Row("Texture", width);
            int texcoord = mCurrentTile->texcoord;
            if (ImGui::InputInt("##ctile-texture", &texcoord, 1))
            {
                auto size = mTileMap->getTexture(mCurrentTile->texture)->uvs.size();
                mCurrentTile->texcoord = LIMIT(texcoord, 0, (int)size - 1);
            }
            ImGui::Row("Textures", width);
            if (ImGui::ComboBox("##mtile-texture", mTileMap->mTextures, mCurrentTile->texture)) {
                mCurrentTile->texcoord = 0;
            }
        }
    }

    void TileMapPanel::placeMode(float width)
    {
        static float scale = 1.0f;
        ImGui::Row("Rotation", width);
        if (ImGui::InputFloat("##tm-r", &mRotation, 45.0f, 90.0f, "%0.0f")) {
            mRotation = LIMIT(mRotation, 0.0f, 360.0f);
        }

        ImGui::Row("Scale", width);
        ImGui::DragFloat("##sc-tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
        ImGui::DrawTexture(mTileMap->getTexture(mCurrentTexture), 0, 0, scale, &mTempTiles);

        if (mConfig->isHover) {
            renderTemp();
        }
        else {
            mConfig->mRender.mTotalTemp = 0;
        }
    }
}