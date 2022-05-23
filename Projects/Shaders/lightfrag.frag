in vec2 uv;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

void main(){
    vec2 mpos = u_mouse.xy/u_resolution.xy;
    float dist = length(mpos);

    gl_FragColor = vec4(1.0, 1.0, 1.0, 1. * (1.0-dist));
}