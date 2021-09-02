#include "TTF.h"
#include <Utils/FileIO.h>
#include <glm/glm.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

namespace Plutus
{
    void TTF::init(char* fontPath, int fontSize)
    {
        unsigned char temp_bitmap[512 * 512];
        auto buffer = readFile(fontPath);

        // stbtt_BakeFontBitmap(buffer.data(), 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
        // can free ttf_buffer at this point

        int BPP;
        int width;
        int height;
        stbi_flip_vertically_on_write(true);
        uint8_t* out = stbi_load("STB2.png", &width, &height, &BPP, 4);
        glGenTextures(1, &mTexId);

        glBindTexture(GL_TEXTURE_2D, mTexId);
        // can free temp_bitmap at this point
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, out);

        glBindTexture(GL_TEXTURE_2D, 0);
        if (out)
        {
            stbi_image_free(out);
        }
    }

    void TTF::print(float x, float y, char* text, SpriteBatch2D& render)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_TEXTURE_2D);
        // glBindTexture(GL_TEXTURE_2D, mTexId);
        // glBegin(GL_QUADS);
        render.begin();
        // while (*text) {
        //     if (*text >= 32 && *text < 128) {
        //         stbtt_aligned_quad q;
        //         stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
        //         // glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
        //         // glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
        //         // glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
        //         // glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        //         float w = q.x1 - q.x0;
        //         float h = q.y1 - q.y0;

        //         /* t0,s0 and t1,s1 are texture-space coordinates, that is floats from
        //  * 0.0-1.0. we have to scale them back to the pixel space used in the
        //  * glyph data bitmap. its a simple as multiplying by the glyph bitmap
        //  * dimensions */
        //  // glm::vec4 src = { q.x0, q.y0, w, h };
        glm::vec4 src = { 0,0, 512, 512 };

        //         /* in gl/d3d the y value is inverted compared to what sdl expects. y0
        //          * is negative here. we add (subtract) it to the baseline to get the
        //          * correct "top" position to blit to */
        //         float uvw = q.s1 - q.s0;
        //         float uvh = q.t1 - q.t0;
        //         glm::vec4 dest = { q.s0, q.t0, uvw, uvh };
        render.submit(mTexId, src);
        //     }
        //     ++text;
        // }
        render.end();
    }


} // namespace Plutus