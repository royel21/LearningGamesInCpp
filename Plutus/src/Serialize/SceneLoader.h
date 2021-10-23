#pragma once
#include <string>

namespace Plutus
{
    class Scene;

    class SceneLoader
    {
    public:
        static bool loadFromJson(const char* path, Scene* scene);
        static bool loadFromString(const std::string& jsonData, Scene* scene);
    };
} // namespace Plutus