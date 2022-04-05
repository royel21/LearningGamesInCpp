#pragma once
#include "Asset.h"

#include <Math/Vectors.h>

namespace Plutus
{
    struct CharData
    {
        float ax; // advance x distance for the next letter
        float ay; // advance y distance for the next letter

        float bw; // width of the letter in pixel ;
        float bh; // height of the letter in pixel;

        float bl; // bitmap left;
        float bt; // bitmap top;

        vec4f uv; // x offset of glyph in texture coordinates
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
        unsigned int mTexId;
        CharData ch[128];

        Font(const char* path, unsigned int fontSize);
        void destroy() override;
    };
} // namespace Plutus