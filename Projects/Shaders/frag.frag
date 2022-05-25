in vec2 uv;
in vec4 color;

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

uniform bool uHasTex;
uniform sampler2D uSampler;
uniform bool blink;

void main(){
    if(uHasTex){
        vec4 tex=texture(uSampler,uv);
        if(blink){
            if(tex.a<.5f){
                discard;
            }
            gl_FragColor=vec4(1,1,1,abs(sin(10*u_time)));
        }else{
            gl_FragColor=texture(uSampler,uv)*color;
        }
    }else{
        gl_FragColor = color;
    }
}