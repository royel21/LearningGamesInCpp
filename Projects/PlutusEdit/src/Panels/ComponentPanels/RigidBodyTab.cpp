#include "RigidBodyTab.h"

#include "ComponentUtil.h"
#include <unordered_map>
#include <algorithm>

#include <ECS/Components/RigidBodyComponent.h>

namespace Plutus
{

    std::unordered_map<std::string, BodyType> rigidBodyTypes = {
            {"Static Body", StaticBody},
            {"Dynamic Body", DynamicBody},
            {"Kinematic Body", KinematicBody},
    };

    RigidBodyTab::RigidBodyTab()
    {
    }

    std::vector<std::string> fixturesType = {
        "Edge","Circle","Box"
    };

    bool drawFixture(Fixture& fix, int id);

    bool CollapseTab(const char* label, int id, bool& remove) {
        bool isOpen = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 25);
        ImGui::PushID(id);
        if (ImGui::TransparentButton(ICON_FA_TRASH_ALT, false, { 1,0,0,1 })) {
            isOpen = false;
            remove = true;
        }
        ImGui::PopID();
        return isOpen;
    }

    void RigidBodyTab::draw(Entity* ent)
    {
        mRigidBody = ent->getComponent<RigidBodyComponent>();
        if (CollapseComponent<RigidBodyComponent>("RigidBody##tilemap-comp", 6)) {
            float textWidth = ImGui::GetContentRegionAvailWidth() * (mTextRowWidth + 0.15f);

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
            if (ImGui::TransparentButton(ICON_FA_PLUS_CIRCLE " Add Fixture##cp")) {
                ImGui::OpenPopup("AddFixture");
            }
            ImGui::Separator();
            if (ImGui::BeginPopup("AddFixture"))
            {
                ImGui::Text("Fixtures");
                ImGui::Separator();
                int type = 0;
                for (auto& fix : fixturesType) {
                    if (ImGui::MenuItem(fix.c_str())) {
                        mRigidBody->mFixtures.push_back({ type });
                    }
                    type++;
                }


                ImGui::EndPopup();
            }
            int id = 0;
            int removeId = -1;
            if (ImGui::BeginChild("fixture-list", { 0, 0 }, false, ImGuiWindowFlags_HorizontalScrollbar)) {
                for (auto& fix : mRigidBody->mFixtures) {
                    if (drawFixture(fix, id++)) {
                        removeId = id - 1;
                    }
                }
            }
            ImGui::EndChild();

            if (removeId > -1) {
                mRigidBody->mFixtures.erase(mRigidBody->mFixtures.begin() + removeId);
            }
        }
    }

    bool RigidBodyTab::drawFixture(Fixture& fix, int index) {
        auto name = fixturesType[fix.type] + "##id-" + std::to_string(index);
        bool remove = false;
        if (CollapseTab(name.c_str(), index, remove)) {
            float textWidth = ImGui::GetContentRegionAvailWidth() * mTextRowWidth;
            ImGui::Row("Offset", textWidth);
            ImGui::Draw2Float("##fix-offset", fix.offset);

            if (fix.type == CircleShape) {
                ImGui::Row("Radius", textWidth);
                ImGui::DragFloat("##fix-radius", &fix.radius, 1, 0.01f, 2000, "%0.2f");
            }
            else {
                ImGui::Row("Size", textWidth);
                ImGui::Draw2Float("##fix-size", fix.size, 1, "W", "H");
            }

            ImGui::Row("friction", textWidth);
            ImGui::DragFloat("##fix-friction", &fix.friction, 0.01f, 0.01f, 5, "%0.2f");
            ImGui::Row("density", textWidth);
            ImGui::DragFloat("##fix-density", &fix.density, 1, 0, 10, "%0.0f");
            ImGui::Row("restitution", textWidth);
            ImGui::DragFloat("##fix-restitution", &fix.restitution, 0.00f, 0.01f, 1, "%0.2f");
            ImGui::Row("isSensor", textWidth);
            ImGui::Checkbox("Is Sensor", &fix.isSensor);
        }

        return remove;
    }
}