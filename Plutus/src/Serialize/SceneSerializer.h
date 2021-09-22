#pragma once
#include <Core/type.h>
#include <string>

namespace Plutus
{
    class Scene;
    class Serializer;

    std::string sceneSerializer(Ref<Scene>& scene);
}