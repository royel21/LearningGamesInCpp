#pragma once

namespace Plutus
{
    class Camera2D;
    struct Project;

    class ISystem
    {
    protected:
        Project* mProject;
        Camera2D* mCamera;

    public:
        ISystem() : mCamera(nullptr) {};
        ISystem(Camera2D* camera) : mCamera(camera) {};
        virtual void init(Project* project) { mProject = project; }
        virtual void update(float dt) {}
        virtual void destroy() {}
    };

} // namespace Plutus
