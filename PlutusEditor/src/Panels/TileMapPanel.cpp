#include "TileMapPanel.h"

#include <iostream>
#include <algorithm>

#include <imgui.h>

#include <ECS/Scene.h>
#include <Input/Input.h>
#include <Utils/Utils.h>
#include <Assets/AssetManager.h>
#include <ECS/Components/TileMap.h>
#include <Platforms/Windows/FileUtils.h>

#include "ImGuiEx.h"
#include "../IconsFontAwesome5.h"

#include "../EditorUI.h"

#include <Graphics/GLSL.h>
#include <Graphics/Camera2D.h>
#include <Graphics/SpriteBatch2D.h>

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

#ifndef _WIN32
#define strncpy_s strncpy
#endif

namespace Plutus
{
    void TileMapPanel::setContext(Ref<Scene>& scene, EditorUI* parent)
    {
        mScene = scene;
        mParentUI = parent;
    }

    void TileMapPanel::addTexture(const char* label, bool& open, TileMap* tMap)
    {
        auto& assetsM = AssetManager::get()->mTextures;
        if (open)
        {
            ImGui::SetNextWindowSize(ImVec2(300.0f, 270.0f));

            ImGui::OpenPopup(label);
            static char name[128];
            static std::string path = "", filename = "";
            static Texture* tilesheet = nullptr;
            static int columns, tileWidth, tileHeight;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
            if (ImGui::BeginPopupModal(label, NULL, window_flags))
            {
                ImGui::PushItemWidth(100);
                ImGui::InputText("Name##modal", name, IM_ARRAYSIZE(name));
                ImGui::SameLine();
                static int selectedType = 0;
                const std::vector<std::string> type{ "Texture", "Tilesheet" };
                ImGui::ComboBox("Type", type, selectedType);
                ImGui::PopItemWidth();
                ImGui::Separator();
                if (ImGui::Button("Load From File"))
                {
                    if (Plutus::windowDialog(OPEN_FILE, path))
                    {
                        filename = Utils::getFileName(path);

                        auto ex = Utils::getExtension(path);
                        if (ex.compare("png") != 0)
                        {
                            path = "";
                        }
                        else
                        {

                            if (selectedType == 1)
                            {
                                assetsM.addTexture(name, path);
                            }
                            else
                            {
                                assetsM.addTexture(name, path, columns, tileWidth, tileHeight);
                            }
                        }
                    }
                }
                ImGui::SameLine();
                ImGui::Text(filename.c_str());
                static bool fromList = false;
                ImGui::Checkbox("From List", &fromList);
                if (fromList)
                {
                    auto& textures = assetsM.mTileSets;

                    if (textures.size())
                    {
                        ImGui::Separator();
                        static auto selected = textures.begin()->first;
                        ImGui::ComboBox("Textures", textures, selected);
                        tilesheet = &textures[selected];
                        columns = tilesheet->columns;
                        tileWidth = tilesheet->tileWidth;
                        tileHeight = tilesheet->tileHeight;
                        ZeroMemory(name, 128);
                        strncpy_s(name, tilesheet->name.c_str(), tilesheet->name.size());
                    }
                }
                ImGui::Separator();
                if (selectedType == 1)
                {
                    ImGui::PushItemWidth(100);
                    ImGui::InputInt("Columns", &columns);
                    ImGui::InputInt("Tile Width", &tileWidth);
                    ImGui::InputInt("Tile Height", &tileHeight);
                    ImGui::PopItemWidth();
                    ImGui::Separator();
                }
                auto pos = ImGui::GetContentRegionAvail();
                ImGui::SetCursorPos(ImVec2(195.0f, 240.0f));
                if (ImGui::Button("save##modal"))
                {
                    std::string texName(name);
                    if (!texName.empty())
                    {
                        mTileMap->addTexture(name);
                        open = false;
                        filename = "";
                        path = "";
                        selectedType = 0;
                        columns = 0;
                        tileWidth = 0;
                        tileHeight = 0;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Close##modal"))
                    open = false;

                ImGui::EndPopup();
            }
        }
    }

    void TileMapPanel::draw(TileMap* tileMap)
    {
        mIsOpen = true;
        mTileMap = tileMap;
        int size[] = { mTileMap->mTileWidth, mTileMap->mTileHeight };

        ImGui::PushItemWidth(110);
        ImGui::InputInt("Layer", &mTileMap->mLayer, 1);
        ImGui::PushItemWidth(60);
        if (ImGui::DragInt2("Tile Size", size, 1))
        {
            mTileMap->mTileWidth = size[0];
            mTileMap->mTileHeight = size[1];
        }
        ImGui::PopItemWidth();

        ImGui::Separator();
        ImGui::ComboBox<Texture*>("TileSheet##mttexture", mTileMap->mTextures, mCurrentTexture);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##add-tilesheet"))
        {
            mShowAddModal = true;
        }
        ImGui::PopStyleColor();
        if (mShowAddModal)
        {
            addTexture("Add Texture To TileMap", mShowAddModal, mTileMap);
        }
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_TRASH_ALT " ##remove-tilesheet"))
        {
            if (mTileMap->mTextures.size()) {
                mTileMap->removeTexture(mCurrentTexture);
                auto first = mTileMap->mTextures.begin();
                if (first != mTileMap->mTextures.end()) {
                    mCurrentTexture = first->first;
                }
            }
        }
        ImGui::PopStyleColor();
        if (mTileMap->mTextures.size() > 0)
        {
            ImGui::RadioButton("Place", &mMode, MODE_PLACE);
            ImGui::SameLine();
            ImGui::RadioButton("Edit", &mMode, MODE_EDIT);
            ImGui::SameLine();
            ImGui::RadioButton("Remove", &mMode, MODE_REMOVE);
            ImGui::Separator();
            if (mMode == MODE_PLACE)
            {
                if (ImGui::InputFloat("Rotation", &mRotation, 45.0f, 90.0f, "%0.0f"))
                {
                    mRotation = LIMIT(mRotation, 0.0f, 360.0f);
                }
                if (mTileMap->mTextures.size()) {
                    static float scale = 1.0f;
                    ImGui::DragFloat("Scale##tex", &scale, 0.05f, 0.2f, 6.0f, "%.2f");
                    ImGui::DrawTexture(mTileMap->mTextures[mCurrentTexture], 0, 0, scale, &mTempTiles);
                }
            }
        }

        if (mCurrentTile != nullptr && mMode == MODE_EDIT)
        {
            tileProps();
        }

        //Render Temp Tiles
        bool isHover = mParentUI->isCanvaHover();
        if (isHover && mTempTiles.size() && mMode == MODE_PLACE && isHover && mTileMap->mTextures.size())
        {
            renderTiles();
        }
        else {
            mParentUI->setTotalTemp(0);
        }

        if (isHover && Input::getInstance()->onKeyDown("MouseLeft")) {
            createTiles();
        }
    }

    void TileMapPanel::renderTiles()
    {
        auto gridCoords = mParentUI->getGridCoords();
        int w = mTileMap->mTileWidth;
        int h = mTileMap->mTileHeight;

        auto& tex = mTileMap->mTextures[mCurrentTexture];

        std::vector<Renderable>& renderables = mParentUI->getRenderables();
        if (renderables.size() < mTempTiles.size()) {
            renderables.resize(mTempTiles.size());
        }
        mParentUI->setTotalTemp((int)mTempTiles.size());

        int i = 0;
        for (auto tile : mTempTiles)
        {
            int x = (gridCoords.x * w) + (tile.x * w);
            int y = (gridCoords.y * h) - (tile.y * h);

            renderables[i++] = { tex->texId, { x, y, w, h }, tex->getUV(tile.z), {}, mRotation, false, false, -1, 99, false };
        }
    }

    void TileMapPanel::tileProps()
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

    void TileMapPanel::createTiles()
    {
        if (!mIsOpen)
            return;

        auto mCoords = mParentUI->getGridCoords();
        auto& tiles = mTileMap->mTiles;
        switch (mMode)
        {
        case MODE_PLACE:
        {
            for (auto tile : mTempTiles)
            {
                uint32_t x = mCoords.x + tile.x;
                uint32_t y = mCoords.y - tile.y;

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

            break;
        }
        case MODE_EDIT:
        {
            mCurrentTile = mTileMap->getTile(mCoords);
            break;
        }
        default:
        {
            if (mTileMap->removeTile(mCoords)) {
                mCurrentTile = nullptr;
            }
        }
        }
    }
} // namespace Plutus
