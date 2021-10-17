#pragma once
#include <Core/type.h>
#include <string>

namespace Plutus
{
    class Scene;

    std::string SceneSerializer(Ref<Scene>& scene);
}