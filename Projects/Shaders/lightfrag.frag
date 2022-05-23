in vec2 uv;
in vec4 color;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

void main(){
    float dist = length(uv);

    gl_FragColor = vec4(color.rgb, color.a * (pow(0.01, dist)-0.01));
}