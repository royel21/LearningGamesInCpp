#include "FontManager.h"

#include <vector>
#include <iostream>

#include <Graphics/GLheaders.h>
#include <Utils/FileIO.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Plutus
{
    FontManager::~FontManager()
    {
        cleanUp();
    }

    bool FontManager::addFont(const std::string& Id, const std::string& fontPath, u32 fontSize)
    {
        auto& font = mFonts[Id];
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return false;
        }

        auto buffer = readFile(fontPath.c_str());
        // Load font as face
        FT_Face face;
        if (FT_New_Memory_Face(ft, buffer.data(), (long)buffer.size(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return false;
        }
        //Set font height in pixel
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &font.texId);
        glBindTexture(GL_TEXTURE_2D, font.texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 500, 500, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        int x = 0;
        int y = 0;

        auto g = face->glyph;
        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
                continue;

            if (x + g->bitmap.width > 500) {
                y += fontSize;
                x = 0;
            }
            if (g->bitmap.rows && g->bitmap.width) {
                glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            }

            font.ch[i].ax = static_cast<float>(g->advance.x >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            font.ch[i].ay = static_cast<float>(g->advance.y >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            font.ch[i].bw = static_cast<float>(g->bitmap.width);
            font.ch[i].bh = static_cast<float>(g->bitmap.rows);
            font.ch[i].bl = static_cast<float>(g->bitmap_left);
            font.ch[i].bt = static_cast<float>(g->bitmap_top);
            font.ch[i].uv = { x / 500.0f, y / 500.0f, (x + g->bitmap.width) / 500.0f, (y + g->bitmap.rows) / 500.0f };
            x += g->bitmap.width;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        if (mCurrentFont.empty()) {
            mCurrentFont = Id;
        }
        buffer.clear();
        return true;
    }

    FontTexture* FontManager::getFont(const std::string& id)
    {
        if (mFonts.find(id) == mFonts.end())
            return nullptr;
        return &mFonts[id];
    }

    void FontManager::remove(std::string font)
    {
        auto it = mFonts.find(font);
        if (it != mFonts.end()) {
            mFonts.erase(it);
        }
    }

    void  FontManager::createRenderable(std::vector<Renderable>& renderables,
        const std::string& text, float x, float y, float scale, ColorRGBA8 color)
    {
        auto& font = mFonts[mCurrentFont];
        for (auto i : text) {
            auto& ch = font.ch[i];
            GLfloat xpos = x + ch.bl * scale;
            GLfloat ypos = y - (ch.bh - ch.bt) * scale; // shift the letter down for Top-Left origin camera

            GLfloat w = ch.bw * scale;
            GLfloat h = ch.bh * scale;
            renderables.push_back({ font.texId, { xpos, ypos, w, h }, ch.uv, color });
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += ch.ax * scale;
        }
    }

    void FontManager::cleanUp()
    {
        for (auto f : mFonts) {
            glDeleteTextures(1, &f.second.texId);
        }
    }
} // namespace Plutus