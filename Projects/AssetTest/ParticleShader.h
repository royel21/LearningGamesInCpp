#pragma once
#include <string>

namespace ParticleShader {
    const std::string vertexShader = R"END(
        layout(location = 0) in vec2 vPosition;
        layout(location = 1) in float vSize;
        layout(location = 2) in float vIndex;
        layout(location = 3) in vec4 vColor;

        uniform mat4 uCamera;

        out float index;
        out float size;
        out vec4 pColor;

        void main(){
            index = vIndex;
            size = vSize;
            pColor = vColor;

            gl_Position = vec4(vPosition, 0, 1.0);
        }
    )END";

    const std::string geoShader = R"END(
        layout(points) in;
        layout(triangle_strip, max_vertices = 4) out;

        uniform mat4 uCamera;
        uniform float uTexData[5];

        vec4 coords(float index){
            int columns = int(uTexData[0]);

            vec4 tuv = vec4(0, 0, 1, 1);
            if(columns > 0){
                float x = float(int(index) % int(columns));
                float y = float(int(index) / int(columns));
                
                tuv.x = x * uTexData[1] + (uTexData[3] * (x+1));
                tuv.y = y * uTexData[2] + (uTexData[4] * (y+1));;
                tuv.z = tuv.x + uTexData[1] - uTexData[3];
                tuv.w = tuv.y + uTexData[2] - uTexData[4];
            }

            return tuv;
        }

        in vec4 pColor[];

        out vec2 uv;
        out vec4 color;

        void createVert(float x, float y, vec2 uUv){
            uv = uUv;
            color = pColor[0];
            gl_Position = uCamera * (gl_in[0].gl_Position + vec4(x, y, 0.0, 0.0));
            EmitVertex();
        }

        in float index[];
        in float size[];

        void main()
        {
            vec4 uvCoords = coords(index[0]);
            float pSize = size[0];
            float halfSize = size[0]/2.0;

            createVert(-halfSize, pSize,  uvCoords.xy);
            createVert(-halfSize, 0, vec2(uvCoords.x, uvCoords.w));
            createVert(halfSize, pSize, vec2(uvCoords.z, uvCoords.y));
            createVert(halfSize, 0,  uvCoords.zw);

            EndPrimitive();
        }
    )END";

    const std::string fragShader = R"END(
        in vec2 uv;
        in vec4 color;

        uniform int uhasTexture;
        uniform sampler2D uSampler;

        out vec4 fragColor;

        void main(){
            if(uhasTexture == 1){
                vec4 tex = texture(uSampler, uv);
                fragColor = tex * color;
            }else{
                fragColor = color;
            }
        }
    )END";
}
