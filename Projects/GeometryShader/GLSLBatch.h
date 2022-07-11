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

        uniform bool uHasTex;
        uniform sampler2D uSampler;

        out vec4 fragColor;

        void main() {

        vec4 tex = texture(uSampler, uv);

        fragColor = uHasTex ? color * tex : color;

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
        uniform sampler2D uSampler;

        out vec4 fragColor;

        void main(){
            vec4 tex = texture(uSampler, uv);

            // if(texIndex == 0){
            //     tex = texture(uSampler[0], uv);
            // }else
            // if(texIndex == 1){
            //     tex = texture(uSampler[1], uv);
            // }else
            // if(texIndex == 2){
            //     tex = texture(uSampler[2], uv);
            // }else
            // if(texIndex == 3){
            //     tex = texture(uSampler[3], uv);
            // }else
            // if(texIndex == 4){
            //     tex = texture(uSampler[4], uv);
            // }else
            // if(texIndex == 5){
            //     tex = texture(uSampler[5], uv);
            // }else
            // if(texIndex == 6){
            //     tex = texture(uSampler[6], uv);
            // }else
            // if(texIndex == 7){
            //     tex = texture(uSampler[7], uv);
            // }else
            // if(texIndex == 8){
            //     tex = texture(uSampler[8], uv);
            // }else
            // if(texIndex == 9){
            //     tex = texture(uSampler[9], uv);
            // }else
            // if(texIndex == 10){
            //     tex = texture(uSampler[10], uv);
            // }else
            // if(texIndex == 11){
            //     tex = texture(uSampler[11], uv);
            // }else
            // if(texIndex == 12){
            //     tex = texture(uSampler[12], uv);
            // }else
            // if(texIndex == 13){
            //     tex = texture(uSampler[13], uv);
            // }else
            // if(texIndex == 14){
            //     tex = texture(uSampler[14], uv);
            // }else
            // if(texIndex == 15){
            //     tex = texture(uSampler[15], uv);
            // }
            if(tex.a < 0.2) discard;
            fragColor = tex * uColor;
        }
    )END";
    const std::string textSDFFrag = R"END(
        in vec2 uv;
        in vec4 color;
        
        uniform sampler2D mySampler;

        const float width = 0.4;
        const float edge = 0.1;

        const float borderWidth = 0.4;
        const float borderEdge = 0.45;

        const vec3 outlineColor = vec3(0.0, 0.0, 0.0);

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
    const std::string textFrag = R"END(
    in vec2 uv;
    in vec4 color;
    out vec4 fragColor;
    uniform sampler2D mySampler;
    void main(){
        vec4 tex = texture(mySampler, uv);
         
        fragColor = vec4(color.rgb, smoothstep(.25,.35, tex.r));
    }
    )END";

} // namespace GLSLBatch
