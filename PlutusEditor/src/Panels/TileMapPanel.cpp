#include "TileMapPanel.h"
#include "ECS/Components/TileMap.h"
#include "imgui.h"
#include "ImGuiEx.h"
#include <iostream>
#include <algorithm>
#include "ECS/Scene.h"
#include "Utils/Utils.h"
#include "Assets/Textures.h"
#include "../IconsFontAwesome5.h"
#include <Platform/Windows/FileUtils.h>

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
    TileMapPanel::~TileMapPanel()
    {
        mShader.dispose();
    }

    void TileMapPanel::setContext(Ref<Scene> &scene)
    {
        mScene = scene;
        mShader.CreateProgWithShader(vertexShader2, fragShader2);
        mRenderer.init(mScene->getCamera());
    }

    void TileMapPanel::addTexture(const char *label, bool &open, TileMap *tMap)
    {
        auto assestM = Textures::get();
        if (open)
        {
            ImGui::SetNextWindowSize(ImVec2(300.0f, 270.0f));

            ImGui::OpenPopup(label);
            static char name[128];
            static std::string path = "", filename = "";
            static Texure *tilesheet = nullptr;
            static int columns, tileWidth, tileHeight;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
            if (ImGui::BeginPopupModal(label, NULL, window_flags))
            {
                ImGui::PushItemWidth(100);
                ImGui::InputText("Name##modal", name, IM_ARRAYSIZE(name));
                ImGui::SameLine();
                static int selectedType = 0;
                const std::vector<std::string> type{"Texture", "Tilesheet"};
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
                                assestM->addTexture(name, path);
                            }
                            else
                            {
                                assestM->addTexture(name, path, columns, tileWidth, tileHeight);
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
                    auto &textures = Textures::get()->mTileSets;

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
                        mTileMap->mTextures.push_back(assestM->getTexture(name));
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

    void TileMapPanel::draw(TileMap *tileMap)
    {
        if (ImGui::CollapsingHeader("TileMap##comp"))
        {
            mIsOpen = true;
            mTileMap = tileMap;
            int size[] = {mTileMap->mTileWidth, mTileMap->mTileHeight};
            ImGui::PushItemWidth(60);
            if (ImGui::DragInt2("Tile Size", size, 1))
            {
                mTileMap->mTileWidth = size[0];
                mTileMap->mTileHeight = size[1];
            }
            ImGui::PopItemWidth();

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
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
            if (mTileMap->mTextures.size() > 0)
            {
                ImGui::PushItemWidth(110);
                ImGui::ComboBox<Texure>("TileSheet##mttexture", mTileMap->mTextures, mCurrentTexture);
                ImGui::PopItemWidth();
                ImGui::Separator();

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
                    ImGui::Texure(mTileMap->mTextures[mCurrentTexture], 1, mTempTiles);
                }

                if (mCurrentTile != nullptr && mMode == MODE_EDIT)
                {
                    tileProps();
                }
            }
        }
        else
        {
            mIsOpen = false;
        }
    }

    void TileMapPanel::renderTiles(SpriteBatch2D *renderer, const glm::ivec2 &mCoords)
    {
        if (mIsOpen && mTempTiles.size() && mMode == MODE_PLACE)
        {
            // mShader.enable();
            // mShader.setUniform1i("hasTexture", 0);
            // mShader.setUniform1i("mySampler", 0);
            // mShader.setUniformMat4("camera", mScene->getCamera()->getCameraMatrix());
            // mRenderer.begin();
            mScene->enableShader();
            renderer->begin();

            std::vector<Tile> tiles;
            int w = mTileMap->mTileWidth;
            int h = mTileMap->mTileHeight;
            for (auto tile : mTempTiles)
            {
                int x = mCoords.x + (tile.x * w);
                int y = mCoords.y + (tile.y * h);

                auto &tex = mTileMap->mTextures[mCurrentTexture];
                // mRenderer.submit(tex->mTexture.id, rect, tex->getUV(tile.z), {}, mRotation);
                if (x < 0)
                {
                    x = x;
                }
                renderer->submit(tex->texId, {x, y, w, h}, tex->getUV(tile.z), {}, mRotation);
            }
            renderer->end();
            mScene->enableShader();
            // mRenderer.end();
            // mShader.disable();
        }
    }

    void TileMapPanel::tileProps()
    {
        ImGui::PushItemWidth(85);
        ImGui::Text("Tile Props");
        ImGui::Separator();
        ImGui::InputInt("X##ctile", &mCurrentTile->x, mTileMap->mTileWidth);
        ImGui::InputInt("Y##ctile", &mCurrentTile->y, mTileMap->mTileHeight);

        ImGui::Checkbox("FlipX##cTile", &mCurrentTile->flipX);
        ImGui::SameLine();
        ImGui::Checkbox("FlipY##cTile", &mCurrentTile->flipY);

        float rotation = mCurrentTile->rotate;
        if (ImGui::InputFloat("Rotation##ctile", &rotation, 45.0f, 90.0f, "%0.0f"))
        {
            mCurrentTile->rotate = LIMIT(rotation, 0.0f, 360.0f);
        }

        int texcoord = mCurrentTile->texcoord;
        if (ImGui::InputInt("Texture##ctile", &texcoord, 1))
        {
            mCurrentTile->texcoord = LIMIT(texcoord, 0, mTileMap->mTileset->totalTiles - 1);
        }
        ImGui::PopItemWidth();
    }

    void TileMapPanel::createTiles(const glm::ivec2 &mCoords)
    {
        if (!mIsOpen)
            return;

        auto tiles = &mTileMap->mTiles;
        switch (mMode)
        {
        case MODE_PLACE:
        {
            for (auto tile : mTempTiles)
            {
                uint32_t x = mCoords.x + (tile.x * mTileMap->mTileWidth);
                uint32_t y = mCoords.y + (tile.y * mTileMap->mTileHeight);

                Tile tile(x, y, tile.z, mCurrentTexture, mRotation);
                int index = mTileMap->getIndex({x, y});
                if (index == -1)
                {
                    tiles->push_back(tile);
                }
                else if (mTileMap->mTiles[index].texcoord != tile.texcoord)
                {
                    mTileMap->mTiles[index] = tile;
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
            mTileMap->removeTile(mCoords);
            if (mCurrentTile != nullptr && mCurrentTile->x == mCoords.x && mCurrentTile->y == mCoords.y)
            {
                mCurrentTile = nullptr;
            }
        }
        }
    }

    bool TileMapPanel::compare(const glm::ivec2 &a, const glm::ivec2 &b)
    {
        if (a.x < b.x)
            return true;
        if (a.x > b.x)
            return false;
        if (a.y < b.y)
            return true;
        if (a.y > b.y)
            return false;
        return false;
    }
} // namespace Plutus
