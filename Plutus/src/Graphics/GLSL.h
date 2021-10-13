#pragma once

#include <string>

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#define VERTEX_HEADER "#version 300 es \nprecision mediump float;\n"
#else
#define VERTEX_HEADER "#version 330 core\n"
#endif

namespace GLSL {

    const std::string vertexShader = std::string(VERTEX_HEADER) + R"END(
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in float entId;

out vec2 uv;
out vec4 color;
out float entityId;

uniform mat4 camera;
uniform vec2 offset;

void main() {
    //Set the x,y position on the screen
    vec2 pos = vertexPosition + offset;
    gl_Position.xy = (camera * vec4(pos, 0, 1.0)).xy;

    gl_Position.z = 0.0;

	gl_Position.w = 1.0;
    
    uv = vertexUV;
    
    color = vertexColor;

    entityId = entId + 1.0;
})END";

    const std::string fragShader = std::string(VERTEX_HEADER) + R"END(
in vec2 uv;
in vec4 color;
in float entityId;

uniform bool isText;
uniform bool picking;
uniform int hasTexture;
uniform sampler2D mySampler;

out vec4 fragColor;

void main() {

    if(isText){
        fragColor = vec4(texture(mySampler, uv).r) * color;
    } else if(picking){
        if(hasTexture > 0){
            vec4 textColor = vec4(1,1,1,1);

            textColor = color * texture(mySampler, uv);

            if(textColor.a < 0.5f){
                discard;
            }
        }

        fragColor = vec4(entityId, entityId, entityId, 1);
    }else if(hasTexture > 0){
        fragColor = color * texture(mySampler, uv); 
    }else{
        fragColor = color;
    }

})END";

    const std::string pickingFragShader = std::string(VERTEX_HEADER) + R"END(
in vec2 uv;
in vec4 color;
in float entityId;

uniform bool isText;
uniform bool picking;
uniform int hasTexture;
uniform sampler2D mySampler;

out vec3 fragColor;

void main() {

   if(hasTexture > 0){
        vec4 textColor = vec4(1,1,1,1);

        textColor = color * texture(mySampler, uv);

        if(textColor.a < 0.5f){
            discard;
        }
    }

    fragColor = vec3(entityId, entityId, entityId);

})END";

    const std::string debug_vertshader = std::string(VERTEX_HEADER) + R"END(
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

    const std::string debug_fragshader = std::string(VERTEX_HEADER) + R"END(
//The fragment shader operates on each pixel in a given polygon

in vec4 color;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 g_color;

void main() {

    g_color = color;
})END";
}