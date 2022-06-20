
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
    float alpha2;
    float radius=25.;
    
    switch(type){
        case 1:{
            float uSoftness=4./u_resolution.x;
            
            float dist=1.-length(uv);
            alpha=smoothstep(uSoftness,uSoftness*2.,dist);
            break;
        }
        case 2:{
            float mul=.5;
            vec2 pos=(abs(uv)*u_resolution*mul);
            
            alpha=1.-clamp(length(max(pos-(u_resolution*mul-radius),0.))-radius,0.,1.);
            break;
        }
    }
    
    float rx=radius/u_resolution.x*2;
    float ry=radius/u_resolution.y*2;
    
    vec4 color4=vec4(1,1,1,1);
    
    vec2 nuv=abs(uv);
    
    // vec2 pixel=1.-2./u_resolution;
    
    // if(nuv.x>pixel.x||nuv.y>pixel.y||alpha==0){
        //     color4=vec4(1,0,0,1);
    // }
    if(nuv.x>1.-rx&&nuv.y>1.-ry){
        
        color4.a=length(vec2(1.-nuv.x,1.-nuv.y));
    }
    outColor=color4;
}
