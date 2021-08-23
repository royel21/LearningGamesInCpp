#pragma once

namespace Plutus
{
    class Scene;

    class ISystem
    {
    protected:
        Scene *mScene;

    public:
        ISystem(Scene *scene) : mScene(scene){};
        virtual void update(float dt) = 0;
    };

} // namespace Plutus
