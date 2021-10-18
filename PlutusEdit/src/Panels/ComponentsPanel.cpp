#include "ComponentsPanel.h"

#include "../Config.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include <imgui.h>
#include <Input/Input.h>
#include <ECS/Components.h>
#include <Physics/Vectors.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Helpers/Render.h"
#include "ComponentUtil.h"

namespace Plutus
{

    template<typename T>
    void ComponentMenuItem(const char* name) {
        if (!Config::get().mProject->mEnt.hasComponent<T>()) {
            if (ImGui::MenuItem(name))
            {
                Config::get().mProject->mEnt.addComponent<T>();
            }
        }
    }

    void ComponentPanel::DrawComponentsPanel()
    {
        ImGui::Begin("##ComPanel");
        if (Config::get().mProject->mEnt) {
            ImGui::InputString("Name##c-tag", Config::get().mProject->mEnt.getComponent<Tag>()->Name);
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " Add Component##cp")) {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent"))
            {
                ImGui::Text("Componets");
                ImGui::Separator();

                ComponentMenuItem<Transform>("Transform");
                ComponentMenuItem<Sprite>("Sprite");
                ComponentMenuItem<TileMap>("TileMap");
                ComponentMenuItem<Animation>("Animation");
                ComponentMenuItem<Script>("Script");

                ImGui::EndPopup();
            }

            ImGui::Separator();
            auto& ent = Config::get().mProject->mEnt;
            if (ent.hasComponent<Transform>()) DrawTransform();
            if (ent.hasComponent<Sprite>()) DrawSprite();
            if (ent.hasComponent<Animation>()) DrawAnimation();
            if (ent.hasComponent<TileMap>()) mTileMapPanel.DrawTileMapComponet();
            if (ent.hasComponent<Script>()) DrawScript();
        }
        ImGui::End();
    }

    void ComponentPanel::DrawTransform() {
        if (CollapseComponent<Transform>("Transform##comp-comp", 1))
        {
            auto trans = Config::get().mProject->mEnt.getComponent<Transform>();
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

    void ComponentPanel::DrawSprite() {
        if (CollapseComponent<Sprite>("Sprite##sprite-comp", 3))
        {
            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {
                auto sprite = Config::get().mProject->mEnt.getComponent<Sprite>();
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
            }
        }
    }
    void ComponentPanel::DrawAnimation() {

    }
    void ComponentPanel::DrawScript() {

    }
}