#pragma once

namespace Plutus
{
    class Camera2D;
    class SystemManager;

    struct Project;

    class ISystem
    {
    protected:
        Project* mProject;
        Camera2D* mCamera;
        SystemManager* mSysManager;

    public:
        ISystem() : mCamera(nullptr) {};
        ISystem(Camera2D* camera) : mCamera(camera) {};

        virtual void init() = 0;
        virtual void update(float dt) = 0;
        virtual void destroy() {}

        virtual void setProject(Project* project) { mProject = project; }
        virtual void setManager(SystemManager* manager) { mSysManager = manager; }
    };

} // namespace Plutus
