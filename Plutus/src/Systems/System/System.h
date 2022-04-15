#pragma once

namespace Plutus
{
    class Scene;
    class Camera2D;

    class ISystem
    {
    protected:
        Scene* mScene;
        Camera2D* mCamera;

    public:
        ISystem() : mCamera(nullptr) {};
        ISystem(Camera2D* camera) : mCamera(camera) {};
        virtual void init(Scene* scene) { mScene = scene; }
        virtual void update(float dt) {}
        virtual void destroy() {}
    };

} // namespace Plutus
