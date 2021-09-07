#pragma once

#include <string>

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#define VERTEX_HEADER "#version 300 es \nprecision mediump float;\n"
#else
#define VERTEX_HEADER "#version 330 core\n"
#endif

namespace GLSL{

const std::string vertexShader = std::string(VERTEX_HEADER) + R"END(
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;

out vec2 uv;
out vec4 color;

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
})END";

const std::string fragShader = std::string(VERTEX_HEADER) + R"END(
in vec2 uv;
in vec4 color;

uniform bool isText;
uniform int hasTexture;
uniform sampler2D mySampler;

out vec4 fragColor;

void main() {

    if(isText){
        fragColor = vec4(texture(mySampler, uv).r) * color;
    }else if(hasTexture > 0){
        fragColor = color * texture(mySampler, uv); 
    }else{
        fragColor = color;
    }

})END";

const std::string debug_vertshader = std::string(VERTEX_HEADER) + R"END(
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 camera;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (camera * vec4(vertexPosition, 0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;

    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;

    fragmentColor = vertexColor;
    fragmentPosition = vertexPosition;
})END";

const std::string debug_fragshader = std::string(VERTEX_HEADER) + R"END(
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})END";
}