#include "ComponentsTab.h"

#include "../Context.h"

#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include <ECS/Components.h>
#include <glm/gtc/type_ptr.hpp>

namespace Plutus
{
    template<typename T>
    bool CollapseComponent(char* label, int id) {
        bool isOpen = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 20);
        ImGui::PushID(id);
        if (ImGui::TransparentButton(ICON_FA_TRASH_ALT, { 1,0,0,1 })) {
            isOpen = false;
            Context.mEnt.removeComponent<T>();
        }
        ImGui::PopID();
        return isOpen;
    }

    void DrawTransform()
    {
        if (CollapseComponent<Transform>("Transform##comp-comp", 1))
        {
            auto trans = Context.mEnt.getComponent<Transform>();
            float position[] = { trans->x, trans->y };
            float itemWidth = 150;

            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {

                ImGui::BeginCol("Position");
                glm::vec2 pos = { trans->x, trans->y };
                if (ImGui::Draw2Float("Position##ent-pos", pos)) {
                    trans->x = pos.x;
                    trans->y = pos.y;
                }
                ImGui::BeginCol("Size");
                glm::vec2 size = { trans->h, trans->w };
                if (ImGui::Draw2Float("Size##ent-size", size, 1, "W", "H")) {
                    trans->h = (int)size.x;
                    trans->w = (int)size.y;
                }
                ImGui::BeginCol("Rotation");
                ImGui::DragFloat("#tran-r", &trans->r, 5, 0, 360, "%.0f");

                ImGui::BeginCol("Layer");
                ImGui::InputInt("#trans-Layer", &trans->layer, 1);

                ImGui::BeginCol("Sort Y");
                ImGui::Checkbox("##trans-sortY", &trans->sortY);
                ImGui::EndUIGroup();
            }
        }
    }

    void DrawSprite()
    {
        if (CollapseComponent<Sprite>("Sprite##sprite-comp", 3))
        {
            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {
                auto sprite = Context.mEnt.getComponent<Sprite>();
                auto& textures = AssetManager::get()->mTextures.mTileSets;

                auto color = sprite->mColor;
                static std::string selected = sprite->mTextureId;
                static int sc = 100;
                static float scale = 1.0f;
                ImGui::BeginCol("TileSheet");
                if (ImGui::ComboBox<Texture>("#TileSheet", textures, selected)) {
                    sprite->mTextureId = selected;
                }
                ImGui::BeginCol("Color");
                if (ImGui::ColorInt("##spr-Color", color))
                {
                    sprite->mColor = color;
                }
                ImGui::BeginCol("Scale");
                if (ImGui::InputInt("##-sprScale", &sc, 5))
                {
                    sc = LIMIT(sc, 25, 300);
                    scale = sc / 100.0f;
                }
                ImGui::BeginCol("Use Coords");
                ImGui::Checkbox("##spr-usecoords", &sprite->mUseUV);

                bool found = textures.find(selected) != textures.end();
                if (found) {
                    if (!sprite->mUseUV) {
                        int max = textures[selected].uvs.size();
                        if (max) {
                            ImGui::BeginCol("UV");
                            int uvIndex = sprite->mUvIndex;
                            if (ImGui::InputInt("#spr-uv", &uvIndex)) {
                                sprite->mUvIndex = CHECKLIMIT(uvIndex, 0, max - 1);
                            }
                        }
                    }
                    else {
                        static int coords[4] = { 0 };
                        ImGui::BeginCol("Coords");
                        if (ImGui::DragFloat4("#spr-uv", glm::value_ptr(sprite->mUVCoord)), 0.001f) {

                        }
                    }
                }
                ImGui::EndUIGroup();
                if (found) {
                    auto& texture = textures[selected];
                    if (sprite->mUseUV) {
                        ImGui::Separator();
                        ImGui::DrawTexCoords(&texture, sprite->mUVCoord);
                    }
                    else {
                        ImGui::DrawTextureOne(&texture, sprite->mUvIndex);
                    }
                }
                // if (found) {
                //     // if (!sprite->mUseUV) {
                //     //     static int coords[4] = { 0 };
                //     //     ImGui::Text("Input Coords In Pixel");
                //     //     ImGui::Separator();
                //     //     ImGui::EndUIGroup();
                //     // }
                //     ImGui::DrawTexture(&textures[selected], 0, 300);
                // }
            }
        }
    }

    void DrawAnimation()
    {

    }

    void DrawScript()
    {

    }

    void DrawTileMap()
    {

    }
}