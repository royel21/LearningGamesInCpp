
in vec2 uv;
in vec4 color;
// in float radius;

uniform vec2 u_resolution;
// uniform float radius;

out vec4 fragColor;

int type=1;

void main()
{
    float alpha;
    float alpha2=0;
    float radius=25;
    float border=0;
    vec4 borderColor=vec4(0,0,0,1);
    
    vec4 color4;
    
    switch(type){
        case 1:{
            float uSoftness=4./u_resolution.x;
            float b=border/u_resolution.x;
            
            float dist=1.-length(uv);
            alpha=smoothstep(uSoftness,uSoftness*2.,dist-b);
            if(border>0){
                alpha2=smoothstep(uSoftness,uSoftness*2.,dist);
            }
            break;
        }
        case 2:{
            vec2 halfSize=u_resolution*.5;
            vec2 pos=(abs(uv)*halfSize);
            float rect=length(max(pos-(halfSize-radius),0.))-radius;
            alpha=1.-clamp(rect+border,0.,1.);
            if(border>0){
                alpha2=1.-clamp(rect,0.,1.);
            }
            break;
        }
    }
    
    if(border>0){
        fragColor=vec4(color*alpha)+vec4(borderColor*alpha2);
    }else{
        fragColor=vec4(color.rgb,alpha);
    }
    
}
