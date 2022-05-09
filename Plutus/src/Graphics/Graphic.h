#pragma once
#include <stdint.h>
#include "GLheaders.h"

namespace Plutus
{
    struct Graphic
    {
        inline static uint32_t createVertexBuffer()
        {
            id = 0;
            glGenVertexArrays(1, &id);
            glBindVertexArray(id);
            return id;
        }

        inline static uint32_t createElementBuffer()
        {

            id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            return id;
        }

        inline static uint32_t createVertexArray()
        {

            id = 0;
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            return id;
        }

    private:
        static uint32_t id;
    };

    uint32_t Graphic::id = 0;
} // namespace Plutus