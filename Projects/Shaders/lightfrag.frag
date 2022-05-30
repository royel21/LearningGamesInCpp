in vec2 uv;
in vec4 color;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

void main(){
    vec2 pos = gl_FragCoord.xy/u_resolution;

    float dist = length(uv);
    // if(pos.y < 0.5) discard;
    gl_FragColor = vec4(color.rgb, color.a * smoothstep(1., 0., dist ));
}