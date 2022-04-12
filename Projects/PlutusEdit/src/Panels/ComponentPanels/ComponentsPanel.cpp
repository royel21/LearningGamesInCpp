#include "ComponentsPanel.h"

#include <imgui.h>
#include <Input/Input.h>
#include <ECS/Components.h>
#include <Math/Vectors.h>
#include <glm/gtc/type_ptr.hpp>

#include "ComponentUtil.h"

#include <Assets/Assets.h>

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
        if (ImGui::Begin("##ComPanel")) {
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

                    ComponentMenuItem<TransformComponent>("Transform");
                    ComponentMenuItem<SpriteComponent>("Sprite");
                    ComponentMenuItem<TileMapComponent>("TileMap");
                    ComponentMenuItem<AnimationComponent>("Animation");
                    ComponentMenuItem<ScriptComponent>("Script");
                    ComponentMenuItem<RigidBodyComponent>("RigidBody");

                    ImGui::EndPopup();
                }

                ImGui::Separator();
                if (mEnt.hasComponent<TransformComponent>()) DrawTransform();
                if (mEnt.hasComponent<SpriteComponent>()) DrawSprite();
                if (mEnt.hasComponent<AnimationComponent>()) mAnimation.DrawAnimation(&mEnt);
                if (mEnt.hasComponent<TileMapComponent>()) mTileMapPanel.DrawTileMapComponet();
                if (mEnt.hasComponent<ScriptComponent>()) DrawScript();
                if (mEnt.hasComponent<RigidBodyComponent>()) mRigidBodyTab.draw(&mEnt);
            }
        }
        ImGui::End();
    }

    /***********************************Transform Comopnent********************************************************/
    void ComponentPanel::DrawTransform() {
        if (CollapseComponent<TransformComponent>("Transform##comp-comp", 1))
        {
            auto trans = mEnt.getComponent<TransformComponent>();
            float position[] = { trans->x, trans->y };

            float textWidth = ImGui::GetContentRegionAvailWidth() * mTextColumnWidth;
            ImGui::Row("Position", textWidth);
            vec2f pos = { trans->x, trans->y };
            if (ImGui::Draw2Float("Position##ent-pos", pos)) {
                trans->x = pos.x;
                trans->y = pos.y;
            }
            ImGui::Row("Size");
            vec2f size = { trans->w, trans->h };
            if (ImGui::Draw2Float("Size##ent-size", size, 1, "W", "H")) {
                trans->w = (int)size.x;
                trans->h = (int)size.y;
            }
            ImGui::Row("Rotation", textWidth);
            ImGui::DragFloat("##tran-r", &trans->r, 5, 0, 360, "%.0f");

            ImGui::Row("Layer", textWidth);
            ImGui::InputInt("##trans-Layer", &trans->layer, 1);

            ImGui::Row("Sort Y");
            ImGui::Checkbox("##trans-sortY", &trans->sortY);
        }
    }
    /***********************************Sprite Comopnent********************************************************/
    void ComponentPanel::DrawSprite() {
        if (CollapseComponent<SpriteComponent>("Sprite##sprite-comp", 2))
        {
            float textWidth = ImGui::GetContentRegionAvailWidth() * (mTextColumnWidth + 0.01f);

            auto sprite = mEnt.getComponent<SpriteComponent>();
            auto& textures = AssetManager::get()->getAssets<Texture>();

            auto color = sprite->mColor;
            std::string selected = sprite->mTextureId;
            ImGui::Row("TileSheet", textWidth);
            if (ImGui::ComboBox("##TileSheet", textures, selected)) {
                sprite->mTextureId = selected;
            }
            ImGui::Row("Color", textWidth);
            if (ImGui::ColorInt("##spr-Color", color))
            {
                sprite->mColor = color;
            }
            ImGui::Row("Scale", textWidth);
            ImGui::DragFloat("##sc-spr", &mSpriteScale, 0.01f, 0.2f, 5, "%0.2f");
            ImGui::Row("Use Coords", textWidth);
            ImGui::Checkbox("##spr-usecoords", &mUseCoords);

            auto found = textures.find(selected);

            if (found != textures.end()) {
                auto& texture = *static_cast<Texture*>(found->second);

                if (!mUseCoords) {
                    int max = (int)texture.uvs.size();
                    if (max) {
                        ImGui::Row("UV", textWidth);
                        if (ImGui::InputInt("##spr-uv", &uvIndex)) {
                            uvIndex = CHECKLIMIT(uvIndex, 0, max - 1);
                            sprite->mUVCoord = texture.getUV(uvIndex);
                        }
                    }
                }
                else {
                    ImGui::Row("Coords", textWidth);
                    ImGui::DragFloat4("##spr-uv", &sprite->mUVCoord.x, 0.001f);
                }

                ImGui::Separator();

                if (mUseCoords) {
                    ImGui::DrawTexCoords(&texture, sprite->mUVCoord);
                }
                else {
                    if (ImGui::DrawTextureOne(&texture, uvIndex)) {
                        sprite->mUVCoord = texture.getUV(uvIndex);
                    }
                }
            }
        }
    }

    /***********************************Script Comopnent********************************************************/
    void ComponentPanel::DrawScript() {
        if (CollapseComponent<ScriptComponent>("Script##script-comp", 5))
        {
            auto script = mEnt.getComponent<ScriptComponent>();
            auto files = Utils::listFiles("assets/script", ".lua");
            if (files.size())
            {

                ImGui::Text("Scripts");
                ImGui::Separator();
                int selected = Utils::getIndex(files, script->mScript);

                if (ImGui::TransparentButton(ICON_FA_TRASH "", true, { 1,0,0,1 }) && selected > -1) {
                    script->destroy();
                }

                if (ImGui::ComboBox("##slist", files, selected))
                {
                    script->setScript(files[selected]);
                }
            }
        }
    }
}