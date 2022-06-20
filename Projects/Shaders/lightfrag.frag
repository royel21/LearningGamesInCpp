
in vec2 uv;
in vec4 color;
// in float radius;

uniform vec2 u_resolution;
// uniform float radius;

out vec4 outColor;

int type=2;

void main()
{
    float alpha;
    
    switch(type){
        case 1:{
            float uSoftness=4./u_resolution.x;
            
            float dist=1.-length(uv);
            alpha=smoothstep(uSoftness,uSoftness*2.,dist);
            break;
        }
        case 2:{
            float radius=50.;
            vec2 halfres=u_resolution*.5;
            vec2 pos=(abs(uv)*halfres);
            
            alpha=1.-clamp(length(max(pos-(halfres-radius),0.))-radius,0.,1.);
            
            break;
        }
    }
    
    outColor=vec4(color.rgb,color.a*alpha);
}
