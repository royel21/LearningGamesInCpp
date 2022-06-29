#pragma once

#include <string>

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#define VERTEX_HEADER "#version 300 es \nprecision highp float;\n"
#else
#define VERTEX_HEADER "#version 330 core\n"
#endif

namespace GLSL {

    const std::string vertexShader = R"END(
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in float entId;

out vec2 uv;
out vec4 color;
out float entityId;

uniform mat4 camera;

void main() {
    //Set the x,y position on the screen
    gl_Position = camera * vec4(vertexPosition, 0, 1.0);
    
    uv = vertexUV;
    
    color = vertexColor;

    entityId = entId + 1.0;
})END";

    const std::string fragShader = R"END(
in vec2 uv;
in vec4 color;
in float entityId;

uniform int type;
uniform int hasTexture;
uniform sampler2D mySampler;

out vec4 fragColor;

void main() {

    vec4 tex = texture(mySampler, uv);

    switch(type){
        case 1:{
            fragColor = vec4(tex.r) * color;
            break;
        }
        case 2:{
            if(tex.a < 0.5f){
                discard;
            }
            fragColor = vec4(entityId, entityId, entityId, 1);
            break;
        }
        default:{
            fragColor = hasTexture > 0 ? color * tex : color;
        }
    }

})END";

    const std::string debug_vertshader = R"END(
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 in_color;

out vec2 fragmentPosition;
out vec4 color;

uniform mat4 camera;

void main() {
    //Set the x,y position on the screen
    gl_Position = camera * vec4(vertexPosition, 0, 1.0);

    color = in_color;
})END";

    const std::string debug_fragshader = R"END(
//The fragment shader operates on each pixel in a given polygon

in vec4 color;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 g_color;

void main() {

    g_color = color;
})END";
}