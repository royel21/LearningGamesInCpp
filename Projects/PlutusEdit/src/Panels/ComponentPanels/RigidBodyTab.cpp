#include "RigidBodyTab.h"

#include <algorithm>
#include <unordered_map>

#include "FixturesTabs.h"
#include "ComponentUtil.h"

#include <ECS/Scene.h>
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


    void RigidBodyTab::draw(Entity* ent)
    {
        mRigidBody = ent->getComponent<RigidBodyComponent>();
        if (CollapseComponent<RigidBodyComponent>("Rigid Body##tilemap-comp", 6)) {
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
            drawFixtures(mRigidBody);
        }
    }
}