#pragma once
#include "Core/type.h"

namespace Plutus
{
    class Scene;

    class SceneLoader
    {
    public:
        static bool loadFromJson(const char *path, Ref<Scene> &scene);
    };
} // namespace Plutus