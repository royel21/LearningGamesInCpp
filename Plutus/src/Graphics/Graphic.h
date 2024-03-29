#pragma once
#include <cstdint>
#include "GLheaders.h"

#ifdef _WIN32
#define glClearDepthf glClearDepth
#endif

#ifdef offsetof
#undef offsetof
#endif

#define offsetof(m,n)  (unsigned int) &((((m*)0)->n))

namespace Plutus
{
    struct Graphic
    {
        /**
         * @brief Create a Vertex Array and bind it, Remember to unbined the Vertex Array after this call
         *
         * @return uint32_t Vertex Array Id
         */
        inline static uint32_t createVertexArray()
        {
            uint32_t id = 0;
            glGenVertexArrays(1, &id);
            glBindVertexArray(id);
            return id;
        }

        /**
         * @brief Create a Element Array Buffer and bind it, Remember to unbined the Element Array Buffer after this call
         *
         * @return uint32_t Buffer Element Id
         */
        inline static uint32_t createElementBuffer()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            return id;
        }
        /**
         * @brief Create a Buffer Array object and bind it, Remember to unbined the Array Buffer after this call
         *
         * @return uint32_t Buffer Array Id
         */
        inline static uint32_t createBufferArray()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            return id;
        }

        static void destroy(uint32_t* vertexOjectId, uint32_t* BufferArrId = nullptr, uint32_t* IndexBufferId = nullptr);

        // Bind the buffer buffId and upload the data unbind after data upload
        inline static void uploadBufferData(uint32_t buffId, uint32_t buffsize, const void* buffer, uint32_t drawType = GL_DYNAMIC_DRAW, uint32_t bufferType = GL_ARRAY_BUFFER) {
            glBindBuffer(bufferType, buffId);
            // glBufferData(bufferType, buffsize, buffer, drawType);

            glBufferData(bufferType, buffsize, nullptr, drawType);
            //Upload the data
            glBufferSubData(bufferType, 0, buffsize, buffer);
            glBindBuffer(bufferType, 0);
        }
        //Enable blend mode GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA or addictive GL_SRC_ALPHA, GL_ONE
        inline static void enableBlend(bool additive = false) {
            glEnable(GL_BLEND);
            if (additive) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            }
            else
            {
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
        inline static void setFAttribute(int pos, int size, uint32_t vertexSize, uint32_t offset = 0, GLenum type = GL_FLOAT, GLboolean normalize = GL_FALSE) {
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
        inline static void setIAttribute(int pos, int size, uint32_t vertexSize, uint32_t offset = 0, GLenum type = GL_INT) {
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
            GLuint format = GL_RGB, GLint type = GL_UNSIGNED_BYTE, GLint glFilter = GL_NEAREST,
            uint32_t glTexWrap = GL_CLAMP_TO_EDGE);

        //Set glClearColor - default to WHITE Color
        inline static void setBackgoundColor(float r = 1, float g = 1, float b = 1, float a = 1)
        {
            glClearColor(r, g, b, a);
        }

        inline static void getViewPortSize(int size[4]) {
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
        /**
         * @brief call glActiveTexture 0, bind GL_Texture_2D to 0
         */
        inline static void unBindTexture() { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0); }
        /**
         * @brief call OpenGL Draw Elements
         *
         * @param count total vertex to draw
         * @param offset use to especfy the offset for multiple draw using single buffer object
         * @param mode  Type of draw call GL_TRANGLES, GL_LINES, GL_POINTS, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN etc, default=GL_TRIANGLES.
         * @param datatype index array buffer data type default=GL_UNSIGNED_INT
         */
        inline static void drawElements(uint32_t vertexCount, uint32_t offset = 0, uint32_t mode = GL_TRIANGLES, GLenum datatype = GL_UNSIGNED_INT) {
            glDrawElements(mode, vertexCount, datatype, (void*)(offset * sizeof(GLuint)));
        }
        /**
         * @brief OpenGL glDrawArray
         *
         * @param mode Type of draw call GL_TRANGLES, GL_LINES, GL_POINTS, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN etc.
         * @param numVertices total vertex to draw = buffer vertex size
         * @param firstIndex first index default 0
         */
        inline static void drawArrays(uint32_t mode, GLsizei numVertices, uint32_t firstIndex = 0) {
            glDrawArrays(mode, firstIndex, numVertices);
        }

        static void uploadIndices(uint32_t iboId, uint32_t count);

        inline static void resizeViewport(int w, int h, int x = 0, int y = 0) { glViewport(x, y, w, h); }
    };

    class VertexClass {
        uint32_t mVAO = 0;
        uint32_t mBufferId = 0;
        uint32_t mIBufferId = 0;

    public:
        ~VertexClass() { destroy(); }
        inline void destroy() { Graphic::destroy(&mVAO, &mBufferId, mIBufferId ? &mIBufferId : nullptr); }

        void init(bool unbind = true, bool useIBO = false) {
            mVAO = Graphic::createVertexArray();
            mBufferId = Graphic::createBufferArray();
            if (useIBO) {
                mIBufferId = Graphic::createElementBuffer();
            }
        }
        inline void bind() { Graphic::bind(mVAO, mIBufferId); }
        inline void unBind() { Graphic::unBind(); }

        // Bind the buffer buffId and upload the data unbind after data upload
        inline void uploadBufferData(uint32_t buffId, uint32_t buffsize, const void* buffer, uint32_t drawType = GL_DYNAMIC_DRAW, uint32_t bufferType = GL_ARRAY_BUFFER) {
            bind();
            glBindBuffer(bufferType, buffId);
            glBufferData(bufferType, buffsize, buffer, drawType);
            glBindBuffer(bufferType, 0);
            unBind();
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
        inline void setFAttribute(int pos, int size, uint32_t vertexSize, uint32_t offset = 0, GLenum type = GL_FLOAT, GLboolean normalize = GL_FALSE) {
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
        inline void setIAttribute(int pos, int size, uint32_t vertexSize, uint32_t offset = 0, GLenum type = GL_INT) {
            glEnableVertexAttribArray(pos);
            glVertexAttribIPointer(pos, size, type, vertexSize, (void*)offset);
        }

    };
} // namespace Plutus