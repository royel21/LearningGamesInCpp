layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

uniform mat4 uCamera;

uniform float tw;
uniform float th;
uniform float uvWidth;
uniform float uvHeight;
uniform int uColumns;

out vec4 color;
out vec2 uv;

in float index2[];

void createVert(float x, float y, vec2 uUv){
    uv = uUv;
    gl_Position = uCamera * (gl_in[0].gl_Position + vec4(x, y, 0.0, 0.0));
    EmitVertex();
}

vec4 coords(float index){
    vec4 tuv = vec4(0, 0, 1, 1);
    if(uColumns > 0){
        float x = float(int(index) % uColumns);
        float y = float(int(index) / uColumns);
        
        tuv.x = x * uvWidth;
        tuv.y = y * uvHeight;
        tuv.z = tuv.x + uvWidth;
        tuv.w = tuv.y + uvHeight;
    }

    return tuv;
}

void main()
{
    float index = index2[0];
    vec4 uvCoords = coords(index);
    
    createVert(0, th,  vec2(uvCoords.x, uvCoords.y));
    createVert(0, 0,   vec2(uvCoords.x, uvCoords.w));
    createVert(tw, th, vec2(uvCoords.z, uvCoords.y));
    createVert(tw, 0,  vec2(uvCoords.z, uvCoords.w));

    EmitVertex();

    EndPrimitive();
}