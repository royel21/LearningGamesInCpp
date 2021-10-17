#include "ComponentsPanel.h"

#include "../Context.h"
#include "../Helpers/ImGuiEx.h"
#include "../Helpers/IconsFontAwesome5.h"

#include <imgui.h>
#include <Input/Input.h>
#include <ECS/Components.h>
#include <Physics/Vectors.h>

#include "../Helpers/Render.h"
#include "ComponentsTab.h"

namespace Plutus
{
    void selectAndmoveEnt() {

        static Entity ent;
        static vec2f lastPos;
        static glm::vec2 entLastPos;
        auto currentPos = Context.mVpState.mouseCoords;
        if (Input::get()->onKeyPressed("MouseLeft"))
        {
            lastPos = currentPos;

            ent = Context.mScene->getEntity(Render::get().mFramePicker.getEntId({ lastPos.x, lastPos.y }));

            if (ent) {
                Context.mEnt = ent;
                if (Context.mEnt.hasComponent<Plutus::Transform>()) {
                    auto trans = Context.mEnt.getComponent<Transform>();
                    entLastPos = trans->getPosition();
                }
            }
        }

        else  if (Input::get()->onKeyDown("MouseLeft") && ent)
        {
            if (ent.hasComponent<Plutus::Transform>()) {
                auto trans = ent.getComponent<Plutus::Transform>();
                glm::vec2 result = { currentPos.x - lastPos.x, currentPos.y - lastPos.y };
                result /= Render::get().mCamera.getScale();

                trans->x = entLastPos.x + result.x;
                trans->y = entLastPos.y + result.y;
            }
        }
    }

    void AddComponent() {
        auto& ent = Context.mEnt;
        if (ent && ImGui::BeginPopup("AddComponent"))
        {
            ImGui::Text("Componets");
            ImGui::Separator();
            if (!ent.hasComponent<Transform>()) {
                if (ImGui::MenuItem("Transform"))
                {
                }
            }
            if (!ent.hasComponent<Sprite>()) {
                if (ImGui::MenuItem("Sprite"))
                {
                }
            }
            if (!ent.hasComponent<TileMap>()) {
                if (ImGui::MenuItem("TileMap"))
                {
                }
            }
            if (!ent.hasComponent<Animation>()) {
                if (ImGui::MenuItem("Animation"))
                {
                }
            }
            if (!ent.hasComponent<Script>()) {
                if (ImGui::MenuItem("Script"))
                {
                }
            }

            ImGui::EndPopup();
        }
    }

    void DrawComponentsPanel()
    {
        ImGui::Begin("##ComPanel");
        if (Context.mEnt) {
            ImGui::InputString(Context.mEnt.getComponent<Tag>()->Name);
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " Add Component##cp")) {
                ImGui::OpenPopup("AddComponent");
            }
            AddComponent();
            ImGui::Separator();
            auto& ent = Context.mEnt;
            if (ent.hasComponent<Transform>()) DrawTransform();
            if (ent.hasComponent<Sprite>()) DrawSprite();
            if (ent.hasComponent<Animation>()) DrawAnimation();
            if (ent.hasComponent<TileMap>()) DrawTileMap();
            if (ent.hasComponent<Script>()) DrawScript();
        }
        ImGui::End();

        if (Context.mVpState.isHover) {
            selectAndmoveEnt();
        }
    }

}