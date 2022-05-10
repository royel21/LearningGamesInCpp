layout(location = 0) in vec2 vPosition;
layout(location = 1) in float vIndex;

uniform mat4 uCamera;

out float index;

void main(){
    index = vIndex;
    gl_Position = vec4(vPosition, 0, 1.0);
}