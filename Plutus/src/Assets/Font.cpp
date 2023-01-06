#include "Font.h"

#include <Log/Logger.h>

#include <Utils/FileIO.h>
#include <Graphics/GLheaders.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

constexpr int BITMAPSIZE = 1024;

namespace Plutus
{
    void Font::init(const std::string& path, int fontSize)
    {
        mPath = path;
        mSize = fontSize;
        auto fullPath = (baseDir + mPath);
        if (!FileIO::exists(fullPath)) {
            Logger::error("Font Not Found:%s", fullPath.c_str());
            return;
        }

        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft)) {
            Logger::error("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        auto buffer = FileIO::readFile(fullPath.c_str());
        // Load font as face
        FT_Face face;
        if (FT_New_Memory_Face(ft, buffer.data(), (long)buffer.size(), 0, &face)) {
            Logger::error("ERROR::FREETYPE: Failed to load font path:%s bufSize:%i", path.c_str(), (long)buffer.size());
            return;
        }
        //Set font height in pixel
        FT_Set_Pixel_Sizes(face, 0, (FT_UInt)fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &mTexId);
        glBindTexture(GL_TEXTURE_2D, mTexId);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, BITMAPSIZE, BITMAPSIZE, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        int x = 0;
        int y = 0;
        uint32_t height = 0;

        auto g = face->glyph;
        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
                continue;

            // FT_Render_Glyph(g, FT_RENDER_MODE_SDF); // <-- And this is new

            if (height < g->bitmap.rows) {
                height = g->bitmap.rows;
            }

            if (x + g->bitmap.width > BITMAPSIZE) {
                y += height;
                x = 0;
                height = 0;
            }

            if (g->bitmap.rows && g->bitmap.width) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            }

            ch[i].ax = static_cast<float>(g->advance.x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            ch[i].ay = static_cast<float>(g->advance.y >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            ch[i].bw = static_cast<float>(g->bitmap.width);
            ch[i].bh = static_cast<float>(g->bitmap.rows);
            ch[i].bl = static_cast<float>(g->bitmap_left);
            ch[i].bt = static_cast<float>(g->bitmap_top);
            ch[i].uv = {
                x / (float)BITMAPSIZE,
                y / (float)BITMAPSIZE,
                (x + g->bitmap.width) / (float)BITMAPSIZE,
                (y + g->bitmap.rows) / (float)BITMAPSIZE
            };

            x += g->bitmap.width;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        buffer.clear();
    }

    void Font::destroy()
    {
        if (mTexId != -1)
            glDeleteTextures(1, &mTexId);

        mTexId = -1;
    }
}