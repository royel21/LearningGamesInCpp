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
        ISystem(Scene* scene) : mScene(scene), mCamera(nullptr) {};
        ISystem(Scene* scene, Camera2D* camera) : mScene(scene), mCamera(camera) {};
        virtual void update(float dt) = 0;
    };

} // namespace Plutus
