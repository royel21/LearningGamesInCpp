#pragma once
#include <cstdint>
#include "GLheaders.h"

#ifdef _WIN32
#define glClearDepthf glClearDepth
#endif

namespace Plutus
{
    struct Graphic
    {
        //Remember to unbined the Vertex Array after this call
        inline static uint32_t createVertexArray()
        {
            uint32_t id = 0;
            glGenVertexArrays(1, &id);
            glBindVertexArray(id);
            return id;
        }

        //Remember to unbined the Element Array Buffer after this call
        inline static uint32_t createElementBuffer()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            return id;
        }
        //Remember to unbined the Array Buffer after this call
        inline static uint32_t createBufferArray()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            return id;
        }

        inline static void uploadBufferData(uint32_t bid, uint32_t buffsize, const void* buffer, uint32_t drawType = GL_DYNAMIC_DRAW) {
            glBindBuffer(GL_ARRAY_BUFFER, bid);
            glBufferData(GL_ARRAY_BUFFER, buffsize, buffer, drawType);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        inline static void enableBlend(bool additive = false) {
            glEnable(GL_BLEND);
            if (additive) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }
            else {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

        }

        inline static void disableBlend() {
            glDisable(GL_BLEND);
        }

        inline static void setFAttribute(int pos, int size, uint32_t vertexSize, void* offset = NULL, GLenum type = GL_FLOAT, GLboolean normalize = GL_FALSE) {
            glEnableVertexAttribArray(pos);
            glVertexAttribPointer(pos, size, type, normalize, vertexSize, offset);
        }

        inline static void setIAttribute(int pos, int size, uint32_t vertexSize, void* offset = NULL, GLenum type = GL_INT) {
            glEnableVertexAttribArray(pos);
            glVertexAttribIPointer(pos, size, type, vertexSize, offset);
        }
        /**
         * @brief  create a texure with default format view OpenGL genTexture - Remember to unbined the texture after this call
         *
         *
         * @param w width of the texture
         * @param h height of the texture
         * @param buff buff image buffer type unsigned char
         * @param intFormat intFormat internal format default to GL_RGB
         * @param format format default to GL_RGB
         * @param type defualt to GL_UNSIGNED_BYTE
         * @param minFilter default GL_NEAREST
         * @param glTexWrap default GL_CLAMP_TO_EDGE
         * @return GLuint glTextureID
         */
        static GLuint createTexture(int w, int h, unsigned char* buff, GLuint intFormat = GL_RGB,
            GLuint format = GL_RGB, GLint type = GL_UNSIGNED_BYTE, GLint glFilter = GL_NEAREST, uint32_t glTexWrap = GL_CLAMP_TO_EDGE)
        {
            GLuint id;
            glGenTextures(1, &id);
            //link the image to a texture in the gpu texture array
            glBindTexture(GL_TEXTURE_2D, id);
            //flag to render the image
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glTexWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glTexWrap);

            //Load the image to the memory of the gpu
            glTexImage2D(GL_TEXTURE_2D, 0, intFormat, w, h, 0, format, type, buff);
            return id;
        }
        //Default to WHITE Color
        inline void setBackgoundColor(float r = 1, float g = 1, float b = 1, float a = 1)
        {
            glClearColor(r, g, b, a);
        }

        inline void getViewPortSize(int size[4]) {
            glGetIntegerv(GL_VIEWPORT, size);
        }

    };
} // namespace Plutus