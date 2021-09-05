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

        stbtt_InitFont(&info, buffer.data(), 0);

        stbtt_BakeFontBitmap(buffer.data(), 0, fontSize, temp_bitmap, mTexWidth, mTexHeight, 32, 96, cdata); // no guarantee this fits!
        glGenTextures(1, &mTexId);

        glBindTexture(GL_TEXTURE_2D, mTexId);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, mTexWidth, mTexHeight, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);

        glBindTexture(GL_TEXTURE_2D, 0);

        // *Font = {};
        // Font->BitmapWidth = mTexWidth;
        // Font->BitmapHeight = mTexHeight;
        // Font->Bitmap = (byte*)malloc(mTexWidth * mTexHeight);
        // Font->Size = fontSize;
        // stbtt_fontinfo info;

        // stbtt_InitFont(&info, buffer.data(), 0);

        // stbtt_pack_context PackContext = {};
        // stbtt_PackBegin(&PackContext, temp_bitmap, mTexWidth, mTexHeight, 0, 1, 0);
        // stbtt_PackSetOversampling(&PackContext, 2, 2);
        // stbtt_pack_range Ranges[1] = {};
        // Ranges[0].chardata_for_range = &cdata;
        // Ranges[0].array_of_unicode_codepoints = 0;
        // Ranges[0].first_unicode_codepoint_in_range = 32;
        // Ranges[0].num_chars = 96;
        // Ranges[0].font_size = STBTT_POINT_SIZE(fontSize);
        // stbtt_PackFontRanges(&PackContext, buffer.data(), 0, Ranges, 1);
        // stbtt_PackEnd(&PackContext);

        // int Ascend;
        // stbtt_GetFontVMetrics(&info, &Ascend, 0, 0);
        // float Scale = stbtt_ScaleForPixelHeight(&info, fontSize);
        // float Baseline = Ascend * Scale;

        // glGenTextures(1, &mTexId);
        // glBindTexture(GL_TEXTURE_2D, mTexId);
        // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mTexWidth, mTexHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    }

    std::vector<Renderable> TTF::getRenderable(float x, float y, char* text, float scale, ColorRGBA8 c)
    {
        std::vector<Renderable> renderables;

        while (*text) {
            if (*text >= 32 && *text < 128) {

                stbtt_aligned_quad q;
                float previous_x = x;
                float previous_y = y;
                glm::vec4 src;

                stbtt_GetBakedQuad(cdata, mTexWidth, mTexHeight, *text - 32, &x, &y, &q, 1);
                if (scale != 1.0f) {
                    float x0 = previous_x + (q.x0 - previous_x) * scale;
                    float x1 = previous_x + (q.x1 - previous_x) * scale;
                    float y0 = previous_y + (q.y0 - previous_y) * scale;
                    float y1 = previous_y + (q.y1 - previous_y) * scale;
                    x = previous_x + (x - previous_x) * scale;
                    y = previous_y + (y - previous_y) * scale;
                    float w = x1 - x0;
                    float h = y1 - y0;
                    src = { x0, y0, x1 - x0, y1 - y0 };
                }
                else {
                    src = { q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0 };
                }
                renderables.push_back({ mTexId, src, { q.s0, q.t1, q.s1, q.t0 }, c });
            }
            ++text;
        }
        return renderables;
    }


} // namespace Plutus