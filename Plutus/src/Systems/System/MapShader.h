#pragma once
#include <string>

namespace MapShader {
    const std::string vertexShader = R"END(
        layout(location = 0) in vec2 vPosition;
        layout(location = 1) in float vIndex;

        uniform mat4 uCamera;

        out float index;

        void main(){
            index = vIndex;
            gl_Position = vec4(vPosition, 0, 1.0);
        }
    )END";

    const std::string geoShader = R"END(
        layout(points) in;
        layout(triangle_strip, max_vertices = 5) out;

        uniform mat4 uCamera;

        uniform float tw;
        uniform float th;

        uniform float uTexData[5];

        out vec4 color;
        out vec2 uv;


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

        void createVert(float x, float y, vec2 uUv){
            uv = uUv;
            vec2 pos = gl_in[0].gl_Position.xy * vec2(th, tw);

            gl_Position = uCamera * ((vec4(pos, 0.0, 1.0)) + vec4(x, y, 0.0, 0.0));
            EmitVertex();
        }

        in float index[];

        void main()
        {
            vec4 uvCoords = coords(index[0]);

            createVert(0, th,  uvCoords.xy);
            createVert(0, 0,   vec2(uvCoords.x, uvCoords.w));
            createVert(tw, th, vec2(uvCoords.z, uvCoords.y));
            createVert(tw, 0,  uvCoords.zw);

            EmitVertex();

            EndPrimitive();
        }
    )END";

    const std::string fragShader = R"END(
        in vec2 uv;
        uniform vec4 uColor;
        uniform sampler2D mySampler;

        out vec4 fragColor;

        void main(){
            vec4 tex = texture(mySampler, uv);
            fragColor = tex * uColor;
        }
    )END";
}
