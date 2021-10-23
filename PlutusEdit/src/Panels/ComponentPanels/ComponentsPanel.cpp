#include "ComponentsPanel.h"

#include <imgui.h>
#include <Input/Input.h>
#include <ECS/Components.h>
#include <Math/Vectors.h>
#include <glm/gtc/type_ptr.hpp>

#include "ComponentUtil.h"

#include <Assets/AssetManager.h>

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
        mEnt = Config::get().mProject->mEnt;
        if (Config::get().mProject->mScene->isValid(mEnt)) {
            ImGui::InputString("Name##c-tag", mEnt.getComponent<Tag>()->Name);
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
            if (mEnt.hasComponent<Transform>()) DrawTransform();
            if (mEnt.hasComponent<Sprite>()) DrawSprite();
            if (mEnt.hasComponent<Animation>()) mAnimation.DrawAnimation(&mEnt);
            if (mEnt.hasComponent<TileMap>()) mTileMapPanel.DrawTileMapComponet();
            if (mEnt.hasComponent<Script>()) DrawScript();
        }
        ImGui::End();
    }

    /***********************************Transform Comopnent********************************************************/
    void ComponentPanel::DrawTransform() {
        if (CollapseComponent<Transform>("Transform##comp-comp", 1))
        {
            auto trans = mEnt.getComponent<Transform>();
            float position[] = { trans->x, trans->y };

            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {

                ImGui::BeginCol("Position");
                vec2f pos = { trans->x, trans->y };
                if (ImGui::Draw2Float("Position##ent-pos", pos)) {
                    trans->x = pos.x;
                    trans->y = pos.y;
                }
                ImGui::BeginCol("Size");
                vec2f size = { trans->h, trans->w };
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
    /***********************************Sprite Comopnent********************************************************/
    void ComponentPanel::DrawSprite() {
        if (CollapseComponent<Sprite>("Sprite##sprite-comp", 3))
        {
            if (ImGui::BeginUIGroup(ImGuiTableFlags_SizingFixedFit)) {
                auto sprite = mEnt.getComponent<Sprite>();
                auto& textures = AssetManager::get()->mTextures.mTileSets;

                auto color = sprite->mColor;
                std::string selected = sprite->mTextureId;
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
                ImGui::DragFloat("##sc-spr", &mSpriteScale, 0.01f, 0.2f, 5, "%0.2");
                ImGui::BeginCol("Use Coords");
                ImGui::Checkbox("##spr-usecoords", &mUseCoords);

                bool found = textures.find(selected) != textures.end();
                if (found) {
                    if (!mUseCoords) {
                        int max = (int)textures[selected].uvs.size();
                        if (max) {
                            ImGui::BeginCol("UV");
                            if (ImGui::InputInt("#spr-uv", &uvIndex)) {
                                uvIndex = CHECKLIMIT(uvIndex, 0, max - 1);
                                sprite->mUVCoord = AssetManager::get()->getTexCoords(selected, uvIndex);
                            }
                        }
                    }
                    else {
                        ImGui::BeginCol("Coords");
                        ImGui::DragFloat4("#spr-uv", &sprite->mUVCoord.x, 0.001f);
                    }
                }
                ImGui::EndUIGroup();
                ImGui::Separator();
                if (found) {
                    auto& texture = textures[selected];
                    if (mUseCoords) {
                        ImGui::DrawTexCoords(&texture, sprite->mUVCoord);
                    }
                    else {
                        if (ImGui::DrawTextureOne(&texture, uvIndex)) {
                            sprite->mUVCoord = AssetManager::get()->getTexCoords(selected, uvIndex);
                        }
                    }
                }
            }
        }
    }

    /***********************************Script Comopnent********************************************************/
    void ComponentPanel::DrawScript() {
        if (CollapseComponent<Script>("Script##script-comp", 5))
        {
            auto script = mEnt.getComponent<Script>();
            auto files = Utils::listFiles("assets/script", ".lua");
            if (files.size())
            {

                ImGui::Text("Scripts");
                ImGui::SameLine();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
                int selected = Utils::getIndex(files, script->mScript);
                if (ImGui::ComboBox("##slist", files, selected))
                {
                    script->setScript(files[selected]);
                }
                ImGui::SameLine();
                if (ImGui::TransparentButton(ICON_FA_TRASH "", true, { 1,0,0,1 }) && selected > -1) {
                    script->destroy();
                }
                ImGui::PopStyleVar();
            }
        }
    }
}