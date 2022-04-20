#include "ComponentWindow.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <Input/Input.h>
#include <Math/Vectors.h>
#include <ECS/Components.h>

#include "./ComponentPanels/FixturesTabs.h"
#include "./ComponentPanels/ComponentUtil.h"

#include <Assets/Assets.h>


namespace Plutus
{

    template<typename T>
    void ComponentMenuItem(Config* config, const char* name) {
        if (!config->mProject.mEnt.hasComponent<T>()) {
            if (ImGui::MenuItem(name))
            {
                config->mProject.mEnt.addComponent<T>();
            }
        }
    }

    void ComponentWindow::draw()
    {
        if (ImGui::Begin("##ComPanel")) {
            mEnt = mConfig->mProject.mEnt;
            if (mConfig->mProject.scene->isValid(mEnt)) {
                ImGui::InputString("Name##c-tag", mEnt.getComponent<Tag>()->Name);

                if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " Add Component##cp")) {
                    ImGui::OpenPopup("AddComponent");
                }

                ImGui::Separator();

                if (ImGui::BeginPopup("AddComponent"))
                {
                    ImGui::Text("Componets");
                    ImGui::Separator();

                    ComponentMenuItem<TransformComponent>(mConfig, "Transform");
                    ComponentMenuItem<SpriteComponent>(mConfig, "Sprite");
                    ComponentMenuItem<TileMapComponent>(mConfig, "TileMap");
                    ComponentMenuItem<AnimationComponent>(mConfig, "Animation");
                    ComponentMenuItem<ScriptComponent>(mConfig, "Script");
                    ComponentMenuItem<RigidBodyComponent>(mConfig, "Rigid Body");
                    ComponentMenuItem<PhysicBodyComponent>(mConfig, "Static Body");

                    ImGui::EndPopup();
                }
                if (mEnt.hasComponent<TransformComponent>()) DrawTransform();
                if (mEnt.hasComponent<SpriteComponent>()) DrawSprite();
                if (mEnt.hasComponent<AnimationComponent>()) mAnimation.draw(mConfig);
                if (mEnt.hasComponent<TileMapComponent>()) mTileMapPanel.draw(mConfig);
                if (mEnt.hasComponent<ScriptComponent>()) DrawScript();
                if (mEnt.hasComponent<RigidBodyComponent>()) DrawRigidBody();
                if (mEnt.hasComponent<PhysicBodyComponent>()) DrawPhysicBody();
            }
        }
        ImGui::End();
    }

    /***********************************Transform Comopnent********************************************************/
    void ComponentWindow::DrawTransform() {
        if (CollapseComponent<TransformComponent>("Transform##comp-comp", 1, mConfig))
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
    void ComponentWindow::DrawSprite() {
        if (CollapseComponent<SpriteComponent>("Sprite##sprite-comp", 2, mConfig))
        {
            float textWidth = ImGui::GetContentRegionAvailWidth() * (mTextColumnWidth + 0.01f);

            auto sprite = mEnt.getComponent<SpriteComponent>();
            auto& textures = AssetManager::get()->getAssets<Texture>();

            uint32_t color = sprite->mColor;
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
    void ComponentWindow::DrawScript() {
        if (CollapseComponent<ScriptComponent>("Script##script-comp", 5, mConfig))
        {
            auto script = mEnt.getComponent<ScriptComponent>();
            auto files = Utils::listFiles("assets/script", ".lua");
            auto scripts = AssetManager::get()->getAssets<Script>();


            ImGui::Text("Scripts");
            ImGui::Separator();
            if (ImGui::TransparentButton(ICON_FA_TRASH "", true, { 1,0,0,1 })) {
                script->mScript = "";
            }
            ImGui::ComboBox("##slist", scripts, script->mScript);
        }
    }

    std::unordered_map<std::string, BodyType> rigidBodyTypes = {
            {"Static Body", StaticBody},
            {"Dynamic Body", DynamicBody},
            {"Kinematic Body", KinematicBody},
    };

    void ComponentWindow::DrawRigidBody()
    {
        auto mRigidBody = mEnt.getComponent<RigidBodyComponent>();
        if (CollapseComponent<RigidBodyComponent>("Rigid Body##tilemap-comp", 6, mConfig)) {
            float textWidth = ImGui::GetContentRegionAvailWidth() * 0.45f;

            ImGui::Row("Is Bullet", textWidth);
            ImGui::Checkbox("##isbullet", &mRigidBody->mBullet);
            ImGui::Row("Fixed Rotation", textWidth);
            ImGui::Checkbox("##fixrotate", &mRigidBody->mFixedRotation);

            ImGui::Row("Linear Damping", textWidth);
            ImGui::DragFloat("##l-damping", &mRigidBody->mLinearDamping, 1, 0, 30, "%.2f");
            ImGui::Row("Gravity Scale", textWidth);
            ImGui::DragFloat("##g-scale", &mRigidBody->mGravityScale, 1, 0, 30, "%.2f");

            auto found = std::find_if(rigidBodyTypes.begin(), rigidBodyTypes.end(),
                [=](auto&& rbtype)-> bool { return rbtype.second == mRigidBody->mBodyType;});

            auto current = found->first;
            ImGui::Row("Body Type", textWidth);
            if (ImGui::ComboBox("", rigidBodyTypes, current)) {
                mRigidBody->mBodyType = rigidBodyTypes[current];
            }

            ImGui::Separator();
            drawFixtures(mRigidBody);
        }
    }

    void ComponentWindow::DrawPhysicBody()
    {
        auto  physicBody = mEnt.getComponent<PhysicBodyComponent>();
        if (CollapseComponent<PhysicBodyComponent>("Phyics Body##tilemap-comp", 7, mConfig)) {
            drawFixtures(physicBody);
        }
    }
}