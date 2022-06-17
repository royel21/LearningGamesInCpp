// in vec2 uv;
// in vec4 color;

// uniform vec2 u_mouse;
// uniform vec2 u_resolution;

// void main(){
//     float dist=1.-length(uv);
//     gl_FragColor=vec4(color.rgb,smoothstep(uSoftness,uSoftness * 2.,dist));
// }


in vec2 uv;
in vec4 color;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

uniform float uSoftness;

void main(){
    float dist=1.;

    if(uv.x > .80 && uv.y > 0.8){
        vec2 pos = uv * .8;
        float dist2 = 1.-length(pos);
        dist = smoothstep(uSoftness,uSoftness*2., dist2);
    }

    gl_FragColor=vec4(color.rgb, dist);
}


// const float PI=3.14159265;

// // from https://iquilezles.org/articles/distfunctions
// float udRoundBox(vec2 p,vec2 b,float r){return length(max(abs(p)-b+r,0.))-r;}

// out vec4 fragColor;

// void main()
// {
//     // setup
//     // float iRadius=25;
//     // vec2 halfRes=(.5*u_resolution.xy)+u_mouse;
    
//     // compute box
//     float b=length(max(abs(uv)-.9,0.)) * 10.;
    
//     // colorize (red / black )
//     vec3 c=mix(vec3(1.,0.,0.),vec3(0.,0.,0.),smoothstep(.999, 1., b));
//     if(length(c.xy)<.1)discard;
    
//     fragColor=vec4(c,1.);
// }
