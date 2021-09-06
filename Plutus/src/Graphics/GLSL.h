#pragma once

#include <string>

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#define VERTEX_HEADER "#version 300 es \nprecision mediump float;\n"
#else
#define VERTEX_HEADER "#version 330 core\n"
#endif

inline const std::string vertexShader2 = std::string(VERTEX_HEADER) + R"END(
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

inline const std::string fragShader2 = std::string(VERTEX_HEADER) + R"END(
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
