#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Plutus
{
    void Camera::init(int width, int height)
    {
        mProjection = glm::ortho(0.0f, float(width), 0.0f, float(height));
    }

    glm::mat4 Camera::getCamera()
    {
        mModel = glm::translate(glm::mat4(1.0f), { mPos, 0.0f });
        mModel = glm::scale(mModel, { mScale, mScale, 0.0f });
        return mProjection * mModel;
    }
}