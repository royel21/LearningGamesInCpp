#include <entt.hpp>

namespace Plutus
{
    class System
    {
    protected:
        entt::registry *mRegister = nullptr;

    public:
        System(entt::registry *_register) : mRegister(_register) {}
    };

} // namespace Plutus
