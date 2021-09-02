#pragma once

#include <stb_truetype.h>
#include <Graphics/GLheaders.h>
#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{

    class TTF
    {
    public:
        TTF() = default;
        void init(char* fontPath, int fontSize);
        void print(float x, float y, char* text, SpriteBatch2D& render);

    private:
        // Character Data
        stbtt_bakedchar cdata[96];
        GLuint mTexId;

    };
} // namespace Plutus