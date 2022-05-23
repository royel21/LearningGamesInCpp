in vec2 uv;
in vec4 color;

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;
uniform sampler2D uSampler;

void main(){
    gl_FragColor = texture(uSampler, uv) * vec4(color.rgb, .2);
}