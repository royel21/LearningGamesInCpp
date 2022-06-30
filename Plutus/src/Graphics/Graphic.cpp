#include "Graphic.h"
#include <vector>

#define MIN_SPRITE 60000*6;

namespace Plutus {
    std::vector<GLuint> __glIndices;

    void Graphic::uploadIndices(uint32_t iboId, uint32_t count) {
        count *= 6;

        if (__glIndices.size() < count) {
            __glIndices.reserve(count);

            __glIndices.clear();

            int offset = 0;
            for (size_t i = 0; i < count; i += 6)
            {
                __glIndices.push_back(offset + 0);
                __glIndices.push_back(offset + 1);
                __glIndices.push_back(offset + 2);
                __glIndices.push_back(offset + 2);
                __glIndices.push_back(offset + 3);
                __glIndices.push_back(offset + 0);

                offset += 4;
            }

        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), __glIndices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint Graphic::createTexture(int w, int h, unsigned char* buff, GLuint intFormat,
        GLuint format, GLint type, GLint glFilter, uint32_t glTexWrap)
    {
        GLuint id = 0;
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

    void Graphic::destroy(uint32_t* vertexOjectId, uint32_t* BufferArrId, uint32_t* IndexBufferId) {
        if (vertexOjectId && *vertexOjectId) {
            glDeleteVertexArrays(1, vertexOjectId);
            *vertexOjectId = -1;
        }

        if (BufferArrId && *BufferArrId) {
            glDeleteBuffers(1, BufferArrId);
            *BufferArrId = -1;
        }

        if (IndexBufferId && *IndexBufferId) {
            glDeleteBuffers(1, IndexBufferId);
            *IndexBufferId = -1;
        }
    }
}