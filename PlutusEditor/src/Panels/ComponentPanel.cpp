#include "ComponentPanel.h"
#include "glm/glm.hpp"

#include "Graphics/SpriteBatch2D.h"
#include "Assets/Textures.h"
#include "Input/Input.h"
#include "ImGuiEx.h"

#include "ECS/Components/Animation.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include "ECS/Scene.h"
#include "Utils/Utils.h"

#define COMP_TRASNFORM 0
#define COMP_SPRITE 1
#define COMP_ANIMATE 2

const uint32_t color1 = IM_COL32(50, 50, 50, 255);
const uint32_t color2 = IM_COL32(50, 50, 60, 255);
const uint32_t color3 = IM_COL32(60, 60, 70, 255);

namespace Plutus
{
    void ComponentPanel::setContext(Ref<Scene> &scene)
    {
        mScene = scene;
        mInput = Input::getInstance();
        mTileMapPanel.setContext(scene);

        createComps["Animation"] = [&](bool &open)
        {
            if (ImGui::Button("Create##Comp"))
            {
                open = false;
                mEntity->addComponent<Animation>(mEntity);
            }
        };

        createComps["Transform"] = [&](bool &open)
        {
            // Transform Props
            static float transPos[] = {0, 0};
            static int transSize[] = {0, 0};
            static float transRotate = 0;

            ImGui::DragFloat2("Position##trans-modal", transPos, 1.0f, 0, 0, "%.2f");
            if (ImGui::DragInt2("Size##trans-modal", transSize, 1, 0))
            {
                transSize[0] = LIMIT(transSize[0], 0, 8000);
                transSize[1] = LIMIT(transSize[1], 0, 8000);
            }
            if (ImGui::InputFloat("Rotation", &transRotate))
            {
                transRotate = LIMIT(transRotate, 0.0f, 360.0f);
            }

            if (ImGui::Button("Create##Comp"))
            {
                open = false;
                mEntity->addComponent<Transform>(transPos[1], transPos[0], transSize[0], transSize[1], transRotate);
            }
        };
        createComps["Sprite"] = [&](bool &open)
        {
            static std::string selected = Textures::get()->mTileSets.begin()->first;

            ImGui::ComboBox<TileSet>("TileSheet", Textures::get()->mTileSets, selected);
            if (ImGui::Button("Create##Comp"))
            {
                mEntity->addComponent<Sprite>(selected);
                open = false;
            }
        };
        createComps["TileMap"] = [&](bool &open)
        {
            if (ImGui::Button("Create##Comp"))
            {
                open = false;
                mEntity->addComponent<TileMap>(32, 32);
            }
        };
        createComps["Script"] = [&](bool &open)
        {
            auto files = Utils::listFiles("assets/script", ".lua");
            if (files.size())
            {
                static int selected = 0;
                ImGui::ComboBox("Script", files, selected);
                if (ImGui::Button("Create##Comp"))
                {
                    open = false;
                    mEntity->addComponent<Script>(files[selected], mEntity, mScene.get());
                }
            }
        };
    }

    void ComponentPanel::drawUI(Entity *ent)
    {
        static bool isOpen = true;
        ImGui::Begin("Components", &isOpen, ImGuiWindowFlags_HorizontalScrollbar);
        {
            if (ent != nullptr)
            {
                mEntity = ent;
                static bool showCreate;
                if (!ent->hasComponent<TileMap>() || !ent->hasComponent<Script>())
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
                    if (ImGui::Button(ICON_FA_PLUS_CIRCLE " Add Component##createComp"))
                    {
                        showCreate = true;
                    }
                    showCreateComp(showCreate);
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                }
                drawAnimate();
                drawTransform();
                drawSprite();
                drawScript();
                drawTileMap();
            }
        }
        ImGui::End();
    }

    void ComponentPanel::render(SpriteBatch2D *renderer, glm::vec2 mcoords)
    {
        mTileMapPanel.renderTiles(renderer, mcoords);
    }

    void ComponentPanel::drawAnimate()
    {
        if (mEntity->hasComponent<Animation>())
        {
            if (ImGui::CollapsingHeader("Animation##comp"))
            {
                auto &animate = mEntity->getComponent<Animation>();
                //Animations
                auto &sequences = animate.mSequences;
                static bool newAnin = false;
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
                if (ImGui::Button(ICON_FA_PLUS_CIRCLE "##add-anin"))
                    newAnin = true;
                ImGui::PopStyleColor();

                ImGui::SameLine();
                auto it = sequences.begin();
                static std::string selected = it != sequences.end() ? it->first : "";
                ImGui::PushItemWidth(100);
                ImGui::ComboBox("Sequences", sequences, selected);
                ImGui::PopItemWidth();

                if (newAnin)
                {
                    showDialog("New Sequences", 280.0f, 200.0f);
                    static char name[128];
                    ImGui::InputText("Name##anim", name, IM_ARRAYSIZE(name));
                    ImGui::SetCursorPos({220.0f, 195.0f});
                    if (ImGui::Button("Save"))
                    {
                        // animate.AddSequences(name);
                        newAnin = false;
                    }
                    endDialog(newAnin);
                }

                if (!selected.empty())
                {
                    auto &anim = sequences[selected];
                    //Textures
                    // auto tilesets = mAManager->getTilesets();
                    // ImGui::ComboBox<TileSet>("TileSheet", tilesets, );
                }
            }
        }
    }

    void ComponentPanel::drawTransform()
    {
        if (mEntity->hasComponent<Transform>())
        {
            if (ImGui::CollapsingHeader("Transform##comp"))
            {
                auto &trans = mEntity->getComponent<Transform>();
                float position[] = {trans.x, trans.y};

                ImGui::PushItemWidth(100);
                if (ImGui::DragFloat2("Position X Y", position, 1.0f, 0, 0, "%.2f"))
                {
                    trans.x = position[0];
                    trans.y = position[1];
                }
                int size[] = {trans.h, trans.w};
                if (ImGui::DragInt2("Size W H", size))
                {
                    trans.h = size[0];
                    trans.w = size[1];
                }
                ImGui::DragFloat("Rotation", &trans.r, 5, 0, 360, "%.0f");
                ImGui::PopItemWidth();
            }
        }
    }

    void ComponentPanel::drawSprite()
    {
        if (mEntity->hasComponent<Sprite>())
        {
            if (ImGui::CollapsingHeader("Sprite##comp"))
            {
                auto &sprite = mEntity->getComponent<Sprite>();
                auto &tilesets = Textures::get()->mTileSets;

                auto color = sprite.mColor.rgba;
                static std::string selected = sprite.mTextureId;
                static int sc = 100;
                static float scale = 1.0f;
                ImGui::ComboBox<TileSet>("TileSheet", tilesets, selected);

                if (ImGui::ColorInt("Color", color))
                {
                    sprite.mColor.setColor(color);
                }
                ImGui::PushItemWidth(100);
                if (ImGui::InputInt("Scale", &sc, 5))
                {
                    sc = LIMIT(sc, 25, 300);
                    scale = sc / 100.0f;
                }
                auto tileset = &tilesets[selected];
                uint32_t id = tileset->mTexture.id;

                if (sprite.mTexId != id && id > 0)
                {
                    sprite.mTextureId = selected;
                    sprite.mTexId = id;
                }

                static char text[120];
                snprintf(text, 120, "Texure Size W:%d H:%d", tileset->mTexture.width, tileset->mTexture.height);
                ImGui::Text(text);
                ImGui::Separator();
                if (tileset != NULL)
                {
                    if (ImGui::BeginTabBar("##TabBar"))
                    {
                        if (ImGui::BeginTabItem("Tile Coords"))
                        {
                            drawCanvas(tileset, scale);
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Sprite Sheet"))
                        {
                            drawTexCoords(tileset, scale);
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                }
            }
        }
    }

    void ComponentPanel::drawTileMap()
    {
        if (mEntity->hasComponent<TileMap>())
        {
            mTileMapPanel.draw(&mEntity->getComponent<TileMap>());
        }
    }

    void ComponentPanel::drawScript()
    {
        if (mEntity->hasComponent<Script>())
        {
            if (ImGui::CollapsingHeader("Script##comp"))
            {
                auto &script = mEntity->getComponent<Script>();
                auto files = Utils::listFiles("assets/script", ".lua");
                if (files.size())
                {
                    int selected = Utils::getIndex(files, script.path);
                    if (ImGui::ComboBox("Script", files, selected))
                    {
                        mEntity->removeComponent<Script>();
                        mEntity->addComponent<Script>(files[selected], mEntity, mScene.get());
                    }
                }
            }
        }
    }

    void ComponentPanel::drawCanvas(TileSet *tileset, float scale)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);

        const int w = tileset->mTexture.width;
        const int h = tileset->mTexture.height;
        uint32_t id = tileset->mTexture.id;

        ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
        ImGui::Image((void *)id, ImVec2(w * scale, h * scale));
        {
            auto color = IM_COL32(255, 255, 255, 100);
            if (id)
            {
                drawList->AddRect(cvPos, cvDestEnd, color);
            }

            if (w && tileset->tileHeight)
            {
                float tileWidth = tileset->tileWidth * scale;
                float tileHeight = tileset->tileHeight * scale;

                float textureHeight = h * scale;
                float textureWidth = w * scale;
                int columns = static_cast<int>(textureWidth / tileWidth);
                if (tileWidth)
                {
                    for (float y = 0; y < textureHeight; y += tileHeight)
                    {
                        drawList->AddLine(ImVec2(cvPos.x, cvPos.y + y),
                                          ImVec2(cvDestEnd.x, cvPos.y + y), color, 1.0f);
                    }
                }
                if (tileWidth)
                {
                    for (float x = 0; x < textureWidth; x += tileWidth)
                    {
                        drawList->AddLine(ImVec2(cvPos.x + x, cvPos.y),
                                          ImVec2(cvPos.x + x, cvDestEnd.y), color, 1.0f);
                    }
                }
                //Rect
                static bool mDown = false;
                if (ImGui::IsItemHovered())
                {
                    ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                    float x = floor(mpos_in_canvas.x / tileWidth);
                    float y = floor(mpos_in_canvas.y / tileHeight);
                    ImVec2 start(x * tileWidth + cvPos.x, y * tileHeight + cvPos.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);

                    drawList->AddRect(start, end, IM_COL32(255, 0, 0, 255));

                    if (mInput->onKeyPressed("MouseLeft"))
                    {
                        mDown = true;
                        mSelectedtiles.clear();
                    }

                    if (!mInput->onKeyDown("MouseLeft"))
                    {
                        mDown = false;
                    }

                    if (mDown)
                    {
                        ImVec2 vec(x, y);

                        auto found = std::find_if(mSelectedtiles.begin(), mSelectedtiles.end(),
                                                  [vec](const ImVec2 &m) -> bool
                                                  { return m.x == vec.x && m.y == vec.y; });
                        if (found == mSelectedtiles.end())
                        {
                            mSelectedtiles.push_back(vec);
                        }
                    }
                }

                for (uint32_t i = 0; i < mSelectedtiles.size(); i++)
                {
                    ImVec2 start(mSelectedtiles[i].x * tileWidth + cv_destStart.x, mSelectedtiles[i].y * tileHeight + cv_destStart.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);
                    drawList->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                }
            }
        }
    }

    void ComponentPanel::drawTexCoords(TileSet *tileset, float scale)
    {

        const int w = tileset->mTexture.width;
        const int h = tileset->mTexture.height;
        uint32_t id = tileset->mTexture.id;

        ImDrawList *drawList = ImGui::GetWindowDrawList();
        static ImVector<ImVec2> points;
        if (ImGui::Button("Add Selection"))
        {
            // LOG_I("Selection Added\n");
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Selection"))
        {
            points.clear();
        }
        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);
        ImVec2 cvDestEnd(cvPos.x + w * scale, cvPos.y + h * scale);
        ImGui::Image((void *)id, ImVec2(w * scale, h * scale));
        {
            static ImVec2 StartCoords(0, 0);
            static ImVec2 EndCoords(0, 0);
            static bool firstClick = false;
            static bool SecondClick = false;

            static bool mDown;
            drawList->AddRectFilledMultiColor(cvPos, cvDestEnd, color1, color2, color3, color2);

            auto color = IM_COL32(255, 255, 255, 100);
            if (id)
            {
                drawList->AddImage((void *)id, cvPos, cvDestEnd);
                drawList->AddRect(cvPos, cvDestEnd, color);
            }
            if (ImGui::IsItemHovered())
            {
                ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                if (mInput->onKeyDown("MouseLeft"))
                {
                    if (!mDown)
                    {
                        StartCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                        mDown = true;
                    }
                    drawList->AddRect(ImVec2(cvPos.x + StartCoords.x, cvPos.y + StartCoords.y), ImVec2(cvPos.x + mpos_in_canvas.x, cvPos.y + mpos_in_canvas.y), IM_COL32(255, 255, 255, 255));
                }

                if (!mInput->onKeyDown("MouseLeft"))
                {
                    if (mDown)
                    {
                        EndCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                        points.push_back(StartCoords);
                        points.push_back(EndCoords);
                        mDown = false;
                    }
                }

                if (mInput->onKeyPressed("MouseRight"))
                {
                    if (points.size())
                    {
                        points.pop_back();
                        points.pop_back();
                    }
                }
            }
            else
            {
                mDown = false;
            }

            for (int i = 0; i < points.Size - 1; i += 2)
                drawList->AddRect(
                    ImVec2(cvPos.x + points[i].x, cvPos.y + points[i].y),
                    ImVec2(cvPos.x + points[i + 1].x, cvPos.y + points[i + 1].y),
                    IM_COL32(255, 255, 255, 255));
        }
    }

    bool ComponentPanel::showCreateComp(bool &open)
    {

        auto datas = getCompList();

        bool save = false;
        if (open)
        {
            showDialog("New Component", 230.f, 300.0f);
            static int selected = 0;
            {
                if (datas.size())
                {

                    ImGui::ComboBox("Type", datas, selected);
                    createComps[datas[selected]](open);
                }

                endDialog(open);
            }
        }
        return save;
    }

    std::vector<std::string> ComponentPanel::getCompList()
    {
        std::vector<std::string> datas;
        if (!mEntity->hasComponent<TileMap>())
        {
            if (!mEntity->hasComponent<Animation>())
            {
                datas.push_back("Animation");
            }
            if (!mEntity->hasComponent<Transform>())
            {
                datas.push_back("Transform");
            }
            if (!mEntity->hasComponent<Sprite>())
            {
                datas.push_back("Sprite");
            }
        }
        if (!mEntity->hasComponent<Script>())
        {
            datas.push_back("Script");
        }
        if (datas.size() == 3 && !mEntity->hasComponent<TileMap>())
        {
            datas.push_back("TileMap");
        }
        return datas;
    }

    void ComponentPanel::showDialog(const char *name, float width, float height)
    {
        auto pos = ImGui::GetWindowPos();
        ImGui::SetNextWindowPos({pos.x + 5, pos.y + 60});

        ImGui::SetNextWindowSize({width, height});
        ImGui::OpenPopup(name);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
        ImGui::BeginPopupModal(name, NULL, window_flags);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
    }

    void ComponentPanel::endDialog(bool &show)
    {
        auto size = ImGui::GetWindowSize();
        ImGui::SetCursorPos({size.x - 60.0f, size.y - 35.0f});
        if (ImGui::Button("Cancel##modal-1"))
            show = false;
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }
} // namespace Plutus