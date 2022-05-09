layout(location = 0) in vec2 vPosition;
layout(location = 1) in float vIndex;
layout(location = 2) in float vTIndex;

uniform mat4 uCamera;

out float index;
out float texIndex;

void main(){
    index = vIndex;
    texIndex = vTIndex;
    gl_Position = vec4(vPosition, 0, 1.0);
}