#pragma once
#include <glm/glm.hpp>

namespace Plutus
{
    class Camera
    {
    public:
        Camera() = default;
        void init(int width, int height);

        inline void setScale(float scale = 1) { mScale = scale; }
        inline float getScale() { return mScale; }


        inline void setPos(glm::vec2 pos) { mPos = pos; }
        inline glm::vec2 getPos() { return mPos; }

        glm::mat4 getCamera();

    private:
        float mScale = 1;
        glm::vec2 mPos = { 0, 0 };
        glm::mat4 mProjection;
        glm::mat4 mModel;
    };
} // namespace Plutus