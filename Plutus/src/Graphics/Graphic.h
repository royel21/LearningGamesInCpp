#pragma once
#include <stdint.h>
#include "GLheaders.h"

namespace Plutus
{
    struct Graphic
    {
        inline static uint32_t createVertexArray()
        {
            uint32_t id = 0;
            glGenVertexArrays(1, &id);
            glBindVertexArray(id);
            return id;
        }

        inline static uint32_t createElementBuffer()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            return id;
        }

        inline static uint32_t createArrayBuffer()
        {
            uint32_t id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            return id;
        }
    };
} // namespace Plutus