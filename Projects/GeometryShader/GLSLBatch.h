#include <string>

namespace GLSLBatch
{
    //Vertex Shader for Tile Map
    const std::string tmapVShader = R"END(
        layout(location = 0) in vec2 vPosition;
        layout(location = 1) in vec2 vUV;

        uniform mat4 uCamera;

        out vec2 uv;

        void main(){
            uv = vUV;
            gl_Position = uCamera * vec4(vPosition, 0, 1.0);
        }
    )END";
    //Fragment Shader for Tile Map
    const std::string tmapFShader = R"END(
        in vec2 uv;
        uniform vec4 uColor;
        uniform sampler2D uSampler;

        out vec4 fragColor;

        void main(){
            vec4 tex = texture(uSampler, uv);
            fragColor = tex * uColor;
        }
    )END";


} // namespace GLSLBatch
