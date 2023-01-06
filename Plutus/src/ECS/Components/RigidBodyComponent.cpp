#include "RigidBodyComponent.h"
#include <box2d/box2d.h>

#include <cmath>
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

    Vec2f RigidBodyComponent::getVelocity() {
        auto vel = mBody->GetLinearVelocity();
        return { vel.x , vel.y };
    }

    void RigidBodyComponent::speedLimit() {
        auto vel = getVelocity();

        float x = mMaxVel.x, y = mMaxVel.y;
        if (x > 0 || y > 0) {

            if (x) {
                if (x > 0) {
                    if (vel.x < -x) {
                        vel.x = -x;
                    }
                    else if (vel.x > x) {
                        vel.x = -x;
                    }
                }
            }

            if (y) {
                if (y > 0) {
                    if (vel.y < -y) {
                        vel.y = -y;
                    }
                    else if (vel.y > y) {
                        vel.y = y;
                    }
                }
            }

        }
        mBody->SetLinearVelocity({ std::roundf(vel.x * mSpeedReducctionFactor.x), std::roundf(vel.y * mSpeedReducctionFactor.y) });
    }
} // namespace Plutus
