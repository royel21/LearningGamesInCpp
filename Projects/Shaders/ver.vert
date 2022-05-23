layout(location=0)in vec2 vPos;
layout(location=1)in vec2 vUV;

out vec2 uv;

uniform mat4 uCamera;

void main(){
    uv=vUV;
    gl_Position=uCamera*vec4(vPos,0,1.);
}