layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vUV;

uniform vec4 uColor;
uniform mat4 uCamera;

out vec2 uv;
out vec4 color;

void main(){
    gl_Position = uCamera * vec4(vPosition, 0, 1.0);
    
    uv = vUV;
    color = uColor
}