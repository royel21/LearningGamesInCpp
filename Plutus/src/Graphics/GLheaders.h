#ifndef _GLHEADERS_H
#define _GLHEADERS_H

#pragma warning(disable: 4005)

#if __ANDROID__ || __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#ifdef _WIN32
#define glClearDepthf glClearDepth
#endif

inline void enableBlendMode()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline void setBackgoundColor(float r = 1, float g = 1, float b = 1, float a = 1)
{
    glClearColor(r, g, b, a);
}

/**
 * @brief  create a texure with default format view OpenGL genTexture
 *
 * @param w width of the texture
 * @param h height of the texture
 * @param buff buff image buffer type unsigned char
 * @param intFormat intFormat internal format default to GL_RGB
 * @param format format default to GL_RGB
 * @param type defualt to GL_UNSIGNED_BYTE
 * @param minFilter default GL_NEAREST
 * @param magFilter default GL_NEAREST
 * @return GLuint glTextureID
 */
inline GLuint createTexture(int w, int h, unsigned char* buff, GLuint intFormat = GL_RGB,
    GLuint format = GL_RGB, GLint type = GL_UNSIGNED_BYTE,
    GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST)
{
    GLuint id;
    glGenTextures(1, &id);
    //link the image to a texture in the gpu texture array
    glBindTexture(GL_TEXTURE_2D, id);
    //flag to render the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Load the image to the memory of the gpu
    glTexImage2D(GL_TEXTURE_2D, 0, intFormat, w, h, 0, format, type, buff);
    return id;
}

inline void getViewPortSize(int size[4]) {
    glGetIntegerv(GL_VIEWPORT, size);
}

#endif // !_GLHEADERS_H
