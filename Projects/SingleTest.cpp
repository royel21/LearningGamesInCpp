#include <iostream>
#include <Math/Vectors.h>
#include <Math/PMath.h>

using namespace Plutus;

float clamp(float v, float min, float max) {
    if (v < min) {
        return min;
    }
    else if (v > max) {
        return max;
    }
    return v;
}

int main(int argc, char const* argv[])
{
    Vec2f u_resolution(400, 200);
    float radius = 50.;
    Vec2f halfres = u_resolution * .5;
    for (float i = -1.0; i < 1; i += .01f)
    {
        Vec2f uv(abs(i), abs(i));
        Vec2f halfres = u_resolution * .5;

        Vec2f pos = uv * halfres;
        Vec2f result = pos - (halfres - radius);

        Vec2f m = max(result, 0.);

        float alpha = 1.f - clamp(m.length() - radius, 0., 1.f);

        printf("pos:  %.3f, %.3f - result: %.3f, %.3f - length: %.4f\n", pos.x, pos.y, m.x, m.y, m.length());
        // printf("length: %.4f - alpha: %.4f\n", m.length() - radius, alpha);
    }


    return 0;
}
