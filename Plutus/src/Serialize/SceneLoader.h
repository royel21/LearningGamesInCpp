#pragma once
#include "Core/type.h"
#include <string>

namespace Plutus
{
    class Scene;

    class SceneLoader
    {
    public:
        static bool loadFromJson(const char* path, Ref<Scene>& scene);
        static bool loadFromString(const std::string& jsonData, Ref<Scene>& scene);
    };
} // namespace Plutus