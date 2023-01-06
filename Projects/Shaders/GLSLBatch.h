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
            vec4 tex = texture(uSampler[texIndex], uv);
            fragColor = tex * uColor;
        }
    )END";


} // namespace GLSLBatch
