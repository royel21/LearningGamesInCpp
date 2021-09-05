#pragma once

#include <stb_truetype.h>
#include <Graphics/GLheaders.h>
#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{
    struct Renderable;

    class TTF
    {
    public:
        TTF() = default;
        void init(char* fontPath, float fontSize = 14);
        std::vector<Renderable> getRenderable(float x, float y, char* text, float scale = 1.0f, ColorRGBA8 c = { 0,0,0,255 });
        GLuint getTextureId() { return mTexId; }
    private:
        // Character Data
        stbtt_bakedchar cdata[96];
        stbtt_fontinfo info;
        GLuint mTexId;

    };
} // namespace Plutus