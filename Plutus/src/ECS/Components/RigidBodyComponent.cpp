#include "RigidBodyComponent.h"
#include <box2d/box2d.h>

namespace Plutus
{
    void RigidBodyComponent::ApplyForce(float x, float y) {
        mBody->ApplyForceToCenter({ x, y }, true);
    }

    void RigidBodyComponent::ApplyImpulse(float x, float y) {
        mBody->ApplyLinearImpulseToCenter({ x, y }, true);
    }

    void RigidBodyComponent::setVelocity(float x, float y) {
        mBody->SetLinearVelocity({ x, y });
    }

    vec2f RigidBodyComponent::getVelocity() {
        auto vel = mBody->GetLinearVelocity();
        return { vel.x , vel.y };
    }

    void RigidBodyComponent::speedLimit() {
        float x = mMaxVel.x, y = mMaxVel.y;
        if (x > 0 || y > 0) {
            auto vel = getVelocity();
            if (x) {
                if (x > 0) {
                    if (vel.x < -x) {
                        mBody->SetLinearVelocity({ -x, vel.y });
                    }
                    else if (vel.x > x) {
                        mBody->SetLinearVelocity({ x, vel.y });
                    }
                }
            }

            if (y) {
                if (y > 0) {
                    if (vel.y < -y) {
                        mBody->SetLinearVelocity({ vel.x, -y });
                    }
                    else if (vel.y > y) {
                        mBody->SetLinearVelocity({ vel.x, y });
                    }
                }
            }
        }
    }
} // namespace Plutus
