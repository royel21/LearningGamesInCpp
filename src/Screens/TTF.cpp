#include "TTF.h"
#include <Utils/FileIO.h>
#include <glm/glm.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

constexpr int mTexWidth = 512;
constexpr int mTexHeight = 512;

namespace Plutus
{
    void TTF::init(char* fontPath, float fontSize)
    {
        unsigned char temp_bitmap[mTexWidth * mTexHeight];
        auto buffer = readFile(fontPath);
        stbtt_BakeFontBitmap(buffer.data(), 0, fontSize, temp_bitmap, mTexWidth, mTexHeight, 32, 96, cdata); // no guarantee this fits!
        glGenTextures(1, &mTexId);

        glBindTexture(GL_TEXTURE_2D, mTexId);
        // can free temp_bitmap at this point
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //See https://en.wikipedia.org/wiki/Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, mTexWidth, mTexHeight, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void TTF::print(float x, float y, char* text, SpriteBatch2D& render)
    {
        float scale = 1.0f;
        render.begin();
        while (*text) {
            if (*text >= 32 && *text < 128) {

                stbtt_aligned_quad q;
                float previous_x = x;
                float previous_y = y;

                stbtt_GetBakedQuad(cdata, mTexWidth, mTexHeight, *text - 32, &x, &y, &q, 1);

                float x0 = previous_x + (q.x0 - previous_x) * scale;
                float x1 = previous_x + (q.x1 - previous_x) * scale;
                float y0 = previous_y + (q.y0 - previous_y) * scale;
                float y1 = previous_y + (q.y1 - previous_y) * scale;
                x = previous_x + (x - previous_x) * scale;
                y = previous_y + (y - previous_y) * scale;

                float w = x1 - x0;
                float h = y1 - y0;
                render.submit(mTexId, { x0, y0, w, h }, { q.s0, q.t1, q.s1, q.t0 });
            }
            ++text;
        }
        render.end();
    }


} // namespace Plutus