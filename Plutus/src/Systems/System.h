#include <entt.hpp>

namespace Plutus
{
    class Scene;
    class System
    {
    protected:
        Scene* mScene;

    public:
        System(Scene* scene) : mScene(scene) {};
        virtual void update(float dt) = 0;
    };

} // namespace Plutus
