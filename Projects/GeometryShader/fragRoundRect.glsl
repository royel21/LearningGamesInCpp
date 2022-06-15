const float PI=3.14159265;

// from https://iquilezles.org/articles/distfunctions
float udRoundBox(vec2 p,vec2 b,float r){return length(max(abs(p)-b+r,0.))-r;}

in vec2 fragCoord;

out vec4 fragColor;

void main()
{
    // setup
    float t=1.;
    float iRadius=25.+(100.*t);
    vec2 halfRes=.5*iResolution.xy;
    
    // compute box
    float b=udRoundBox(fragCoord.xy-halfRes,halfRes,iRadius);
    
    // colorize (red / black )
    vec3 c=mix(vec3(1.,0.,0.),vec3(0.,0.,0.),smoothstep(0.,1.,b));
    
    fragColor=vec4(c,1.);
}