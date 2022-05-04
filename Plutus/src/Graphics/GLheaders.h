#ifndef _GLHEADERS_H
#define _GLHEADERS_H

#pragma warning(disable: 4005)

#if __ANDROID__ || __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#endif // !_GLHEADERS_H
