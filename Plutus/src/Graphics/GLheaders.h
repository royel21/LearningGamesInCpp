#ifndef _GLHEADERS_H
#define _GLHEADERS_H

#if __ANDROID__

#include <GLES3/gl3.h>

#elif __EMSCRIPTEN__

#include <GLES3/gl3.h>
#define glClearDepth glClearDepthf

#else

extern "C"
{
#include <windows.h>
}
#include <glad/glad.h>

#endif

inline void enableBlendMode()
{
    glEnable(GL_CULL_FACE | GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline void setBackgoundColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

/*
    create a texure with default format view OpenGL genTexture
    @param w width of the texture
    @param h height of the texture
    @param buff image buffer of byte
    @param intFormat internal format default to GL_RGB
    @param format format default to GL_RGB
    @param type defualt to GL_UNSIGNED_BYTE

*/
inline GLuint createTexture(int w, int h, unsigned char* buff, GLuint intFormat = GL_RGB,
    GLuint format = GL_RGB, GLuint type = GL_UNSIGNED_BYTE, GLuint minFilter = GL_NEAREST,
    GLuint magFilter = GL_NEAREST) {

    GLuint id;
    glGenTextures(1, &id);
    //link the image to a texture in the gpu texture array
    glBindTexture(GL_TEXTURE_2D, id);
    //flag to render the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    //Load the image to the memory of the gpu
    glTexImage2D(GL_TEXTURE_2D, 0, intFormat, w, h, 0, format, type, buff);
    return id;
}

inline void getViewPortSize(int size[4]) {
    glGetIntegerv(GL_VIEWPORT, size);
}

#endif // !_GLHEADERS_H
