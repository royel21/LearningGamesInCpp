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
        //Create a Vertex Array and bind it, Remember to unbined the Vertex Array after this call
        inline static uint32_t createVertexArray()
        {
            uint32_t id = 0;
            glGenVertexArrays(1, &id);
            glBindVertexArray(id);
            return id;
        }

        //Create a Element Array Buffer and bind it, Remember to unbined the Element Array Buffer after this call
        inline static uint32_t createElementBuffer()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            return id;
        }
        //Create a Array Buffer and bind it, Remember to unbined the Array Buffer after this call
        inline static uint32_t createBufferArray()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            return id;
        }

        static void destroy(uint32_t* vertexOjectId, uint32_t* IndexBufferId = nullptr, uint32_t* BufferArrId = nullptr) {
            if (vertexOjectId && *vertexOjectId > 0) {
                glDeleteVertexArrays(1, vertexOjectId);
                *vertexOjectId = -1;
            }

            if (IndexBufferId && *IndexBufferId > 0) {
                glDeleteBuffers(1, IndexBufferId);
                *IndexBufferId = -1;
            }

            if (BufferArrId && *BufferArrId > 0) {
                glDeleteBuffers(1, BufferArrId);
                *BufferArrId = -1;
            }
        }

        // Bind the buffer buffId and upload the data unbind after data upload
        inline static void uploadBufferData(uint32_t buffId, uint32_t buffsize, const void* buffer, uint32_t drawType = GL_DYNAMIC_DRAW, uint32_t bufferType = GL_ARRAY_BUFFER) {
            glBindBuffer(bufferType, buffId);
            glBufferData(bufferType, buffsize, buffer, drawType);
            glBindBuffer(bufferType, 0);
        }
        //Enable blend mode GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA or addictive GL_SRC_ALPHA, GL_ONE
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
        /**
         * @brief Enable VertexAttibute and set vertex attribute float pointer
         *
         * @param pos Layout position on the vertex shader
         * @param size count of elements of this attributes float = 1, vec2 = 2 etc
         * @param vertexSize size in bytes of the struct or class use sizeof()
         * @param offset use offsetof or calculate
         * @param type GLenum TYPE def: GL_FLOAT
         * @param normalize if should be normalize to float def: GL_FALSE
         */
        inline static void setFAttribute(int pos, int size, uint32_t vertexSize, size_t offset = NULL, GLenum type = GL_FLOAT, GLboolean normalize = GL_FALSE) {
            glEnableVertexAttribArray(pos);
            glVertexAttribPointer(pos, size, type, normalize, vertexSize, (void*)offset);
        }
        /**
        * @brief Enable VertexAttibute and set vertex attribute integer pointer
        *
        * @param pos Layout position on the vertex shader
        * @param size count of elements of this attributes float = 1, vec2 = 2 etc
        * @param vertexSize size in bytes of the struct or class use sizeof()
        * @param offset use offsetof or calculate
        * @param type GLenum TYPE def: GL_INT
        */
        inline static void setIAttribute(int pos, int size, uint32_t vertexSize, size_t offset = NULL, GLenum type = GL_INT) {
            glEnableVertexAttribArray(pos);
            glVertexAttribIPointer(pos, size, type, vertexSize, (void*)offset);
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
        //Set glClearColor - default to WHITE Color
        inline void setBackgoundColor(float r = 1, float g = 1, float b = 1, float a = 1)
        {
            glClearColor(r, g, b, a);
        }

        inline void getViewPortSize(int size[4]) {
            glGetIntegerv(GL_VIEWPORT, size);
        }
        //Bind vertex object and buffer element array if provided
        inline static void bind(uint32_t vaoId, uint32_t iboId = 0) {
            glBindVertexArray(vaoId);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
        }
        /**
         * @brief unbind -> bind vertex 0, buffer array 0, buffer element 0 and texture 0
         *
         */
        static void unBind() {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // bind texture to id and texture unit to unit def = 0
        inline static void bindTexture(uint32_t id, uint32_t unit = 0) {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, id);
        }
        inline static void unBindTexture() { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0); }

        inline static void drawElements(uint32_t count, uint32_t offset = 0, uint32_t type = GL_TRIANGLES, GLenum datatype = GL_UNSIGNED_INT) {
            glDrawElements(type, count, datatype, (void*)(offset * sizeof(GLuint)));
        }

    };
} // namespace Plutus