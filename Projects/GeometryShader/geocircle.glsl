layout(points) in;
layout(line_strip, max_vertices = 64) out;

uniform mat4 uCamera;

const float PI = 3.1415926;
const float radius = 50.0;

void main()
{
    for (int i = 0; i < 64; i++) {
        // Angle between each side in radians
        float ang = (i / 63.0) * 2.0 * PI;

        // Offset from center of point
        vec4 offset = vec4(cos(ang) * radius, sin(ang) * radius, 0.0, 0.0);
        gl_Position = uCamera * (gl_in[0].gl_Position + offset);

        EmitVertex();
    }

    EndPrimitive();
}