#pragma once

namespace Plutus
{
    class Camera2D;
    class SystemManager;

    struct Project;

    /**
     * @brief System Interface should implement
     * @param init initializer function
     * @param update(float) called every frame
     * @param draw optional called every frame
     * @param destroy optional callled when system is destroyed
     */
    class ISystem
    {
    protected:
        Project* mProject;
        Camera2D* mCamera;
        SystemManager* mSysManager;

    public:
        ISystem(): mCamera(nullptr) {};
        ISystem(Camera2D* camera): mCamera(camera) {};

        virtual void init() = 0;
        virtual void update(float dt) = 0;
        virtual void draw() {}
        virtual void destroy() {}

        virtual void setProject(Project* project) { mProject = project; }
        virtual void setManager(SystemManager* manager) { mSysManager = manager; }
    };

} // namespace Plutus
