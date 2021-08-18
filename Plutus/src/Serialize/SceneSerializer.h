#pragma once
#include <Core/type.h>

namespace Plutus
{
    class Scene;
    class Serializer;

    void sceneSerializer(Serializer& ser, Ref<Scene>& scene);
}