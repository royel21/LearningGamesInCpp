#pragma once

#include "Asset.h"
#include <Math/Vectors.h>

namespace Plutus
{
    struct CharData
    {
        float ax = 0; // advance x distance for the next letter
        float ay = 0; // advance y distance for the next letter

        float bw = 0; // width of the letter in pixel ;
        float bh = 0; // height of the letter in pixel;

        float bl = 0; // bitmap left;
        float bt = 0; // bitmap top;

        Vec4f uv; // x offset of glyph in texture coordinates
    };
    /*
        Font Struct Replesenting a single font
        Constructor Params
        @param FontId Asset Id
        @param FontPath Path in asset folder
        @param FontSize
    */
    struct Font : public Asset
    {
        unsigned int mTexId = -1;
        int mSize = 16;
        CharData ch[128] = { 0 };

        Font() = default;
        ~Font() { destroy(); }
        //Constructor
        Font(const std::string& path, int fontSize = 16) { init(path, fontSize); }
        void init(const std::string& path, int fontSize = 16);

        void destroy() override;
    };
} // namespace Plutus