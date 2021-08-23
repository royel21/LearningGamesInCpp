#include "Scene.h"
#include <algorithm>

namespace Plutus
{
    template <typename T, typename... Args>
    T& Entity::addComponent(Args &&...args)
    {
        return mScene->mRegistry.emplace_or_replace<T>(mId, std::forward<Args>(args)...);
    }

    template <typename T>
    T& Entity::getComponent()
    {
        return mScene->mRegistry.get<T>(mId);
    }

    template <typename T>
    bool Entity::removeComponent()
    {
        if (hasComponent<T>())
        {
            mScene->mRegistry.remove<T>(mId);
        }
        return 1;
    }

    template <typename T>
    bool Entity::hasComponent()
    {
        return mScene->mRegistry.any_of<T>(mId);
    }
}
// namespace Plutus
