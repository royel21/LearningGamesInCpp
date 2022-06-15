#include <string>

namespace GLSLBatch
{
    const std::string spriteVShader = R"END(
        layout(location = 0) in vec2 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        layout(location = 2) in vec4 vertexColor;

        out vec2 uv;
        out vec4 color;

        uniform mat4 uCamera;

        void main() {
            //Set the x,y position on the screen
            gl_Position = uCamera * vec4(vertexPosition, 0, 1.0);
            
            uv = vertexUV;
            
            color = vertexColor;
        })END";

    const std::string spriteFShader = R"END(
        in vec2 uv;
        in vec4 color;

        uniform int uHasTex;
        uniform sampler2D uSampler;

        out vec4 fragColor;

        void main() {

        vec4 tex = texture(uSampler, uv);

        fragColor = uHasTex > 0 ? color * tex : color;

    })END";

    //Vertex Shader for Tile Map
    const std::string tmapVShader = R"END(
        layout(location = 0) in vec2 vPosition;
        layout(location = 1) in vec2 vUV;
        layout(location = 2) in int vTexIndex;

        uniform mat4 uCamera;

        out vec2 uv;
        flat out int texIndex;

        void main(){
            uv = vUV;
            texIndex = vTexIndex;

            gl_Position = uCamera * vec4(vPosition, 0, 1.0);
        }
    )END";
    //Fragment Shader for Tile Map
    const std::string tmapFShader = R"END(
        in vec2 uv;
        flat in int texIndex;

        uniform vec4 uColor;
        uniform sampler2D uSampler[16];

        out vec4 fragColor;

        void main(){
            vec4 tex;

            switch(texIndex){
                case 0:{
                    tex = texture(uSampler[0], uv);
                    break;
                }
                case 1:{
                    tex = texture(uSampler[1], uv);
                    break;
                }
                case 2:{
                    tex = texture(uSampler[2], uv);
                    break;
                }
                case 3:{
                    tex = texture(uSampler[3], uv);
                    break;
                }
                case 4:{
                    tex = texture(uSampler[4], uv);
                    break;
                }
                case 5:{
                    tex = texture(uSampler[5], uv);
                    break;
                }
                case 6:{
                    tex = texture(uSampler[6], uv);
                    break;
                }
                case 7:{
                    tex = texture(uSampler[7], uv);
                    break;
                }
                case 8:{
                    tex = texture(uSampler[8], uv);
                    break;
                }
                case 9:{
                    tex = texture(uSampler[9], uv);
                    break;
                }
                case 10:{
                    tex = texture(uSampler[10], uv);
                    break;
                }
                case 11:{
                    tex = texture(uSampler[11], uv);
                    break;
                }
                case 12:{
                    tex = texture(uSampler[12], uv);
                    break;
                }
                case 13:{
                    tex = texture(uSampler[13], uv);
                    break;
                }
                case 14:{
                    tex = texture(uSampler[14], uv);
                    break;
                }
                case 15:{
                    tex = texture(uSampler[15], uv);
                    break;
                }
            }
            
            fragColor = tex * uColor;
        }
    )END";
    const std::string textFrag = R"END(
        in vec2 uv;
        in vec4 color;
        
        uniform sampler2D mySampler;

        const float width = 0.5;
        const float edge = 0.05;

        const float borderWidth = 0.2;
        const float borderEdge = 0.45;

        const vec3 outlineColor = vec3(1.0, 0.0, 0.0);

        out vec4 fragColor;

        vec2 offset = vec2(0.000, 0.003);

        void main() {
            float dist = 1.0 - texture(mySampler, uv).r;
            float alpha = 1.0-smoothstep(width, width+edge, dist);


            float dist2 = 1.0 - texture(mySampler, uv+offset).r;
            float outlineAlpha = 1.0-smoothstep(borderWidth, borderWidth+borderEdge, dist2);

            float overalAlpha = alpha + (1.0 - alpha) * outlineAlpha;

            vec3 overalColor = mix(outlineColor,color.rgb, alpha/outlineAlpha);
                
            fragColor = vec4(overalColor, overalAlpha);
        }
    )END";

} // namespace GLSLBatch
