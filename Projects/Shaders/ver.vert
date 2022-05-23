layout(location=0)in vec2 vPos;
layout(location=1)in vec2 vUV;
layout(location=2)in vec4 vColor;

out vec2 uv;
out vec4 color;

uniform mat4 uCamera;

void main(){
    uv=vUV;
    color=vColor;
    gl_Position=uCamera*vec4(vPos,0,1.);
}