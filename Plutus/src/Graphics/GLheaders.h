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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif // !_GLHEADERS_H
