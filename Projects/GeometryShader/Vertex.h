#pragma once

namespace Plutus
{
    struct Vertex
    {
        float x;
        float y;
        float uvx;
        float uvy;
        Vertex() = default;
        Vertex(float x1, float y1, float uv1, float uv2) : x(x1), y(y1), uvx(uv1), uvy(uv2) {};
    };
} // namespace Plutus