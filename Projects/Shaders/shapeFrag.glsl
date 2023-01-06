in vec2 uv;
in vec2 size;
in vec4 color;
in float radius;
flat in int type;

out vec4 fragColor;

uniform float border;
uniform vec4 borderColor;

void main(){
    float alpha=0;
    float alpha2=0;
    
    switch(type){
        case 1:{
            vec2 halfSize=u_resolution*.5;
            vec2 pos=(abs(uv)*halfSize);
            float rect=length(max(pos-(halfSize-radius),0.))-radius;
            
            alpha=1.-clamp(rect+border,0.,1.);
            
            if(border>0){
                alpha2=1.-clamp(rect,0.,1.);
            }
            break;
        }
        default:{
            float uSoftness=4./u_resolution.x;
            float b=border/u_resolution.x;
            
            float dist=1.-length(uv);
            alpha=smoothstep(uSoftness,uSoftness*2.,dist-b);
            
            if(border>0){
                alpha2=smoothstep(uSoftness,uSoftness*2.,dist);
            }
        }
    }
    fragColor=vec4(color*alpha)+vec4(borderColor*alpha2);
}