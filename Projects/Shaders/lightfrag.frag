// in vec2 uv;
// in vec4 color;

// uniform vec2 u_mouse;
// uniform vec2 u_resolution;
// uniform float uSoftness;
// out vec4 outColor;
// void main(){
    //     float dist=1.-length(uv);
    //     outColor=vec4(color.rgb,smoothstep(uSoftness,uSoftness * 2.,dist));
// }

in vec2 uv;
in vec4 color;

uniform vec2 u_mouse;
uniform vec2 u_resolution;

float u_radius=.3;

out vec4 outColor;

void main()
{
    vec2 coords=uv*u_resolution;
    if(length(coords-vec2(0))<u_radius||length(coords-vec2(0,u_resolution.y))<u_radius||length(coords-vec2(u_resolution.x,0))<u_radius||length(coords-u_resolution)<u_radius){
        discard;
    }
    outColor=color;
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
