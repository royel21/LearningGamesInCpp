in vec2 uv;

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
uniform sampler2D uSampler;

void main(){
    gl_FragColor = texture(uSampler, uv);
}