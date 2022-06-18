
in vec2 uv;
in vec4 color;
in float radius;

uniform vec2 u_resolution;

out vec4 outColor;

int type=1;

void main()
{
    float alpha;
    
    // switch(type){
        //     case 1:{
            //         float uSoftness=4./radius;
            
            //         float dist=1.-length(uv);
            //         alpha=vec4(color.rgb,smoothstep(uSoftness,uSoftness*2.,dist));
            //         break;
        //     }
        //     case 2{
            //         float radius=50.;
            //         vec2 pos=(abs(uv))*u_resolution*.5;
            
            //         float alpha=1.-clamp(length(max(pos-(u_resolution*.5-radius),0.))-radius,0.,1.);
            //         break;
        //     }
        //     default:{
            
        //     }
    // }
    float radius=5.;
    vec2 pos=(abs(uv))*u_resolution*.5;
    
    alpha=1.-clamp(length(max(pos-(u_resolution*.5-radius),0.))-radius,0.,1.);
    
    outColor=vec4(color.rgb,alpha);
}
