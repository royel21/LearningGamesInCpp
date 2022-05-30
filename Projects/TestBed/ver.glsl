layout(location=0)in vec2 vPosition;

uniform mat4 uCamera;

void main(){
    gl_Position=uCamera*vec4(vPosition,0,1.);
}