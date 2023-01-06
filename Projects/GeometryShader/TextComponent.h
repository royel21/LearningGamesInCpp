#include <string>

#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

namespace Plutus
{
    struct TextComponent
    {
        Vec2f mOffset;
        ColorRGBA8 mColor;
        std::string mText;
        std::string mFontId;
        float mScale = 1;

        TextComponent() = default;
        TextComponent(const std::string& fontId, const std::string& text, float x = 0, float y = 0, ColorRGBA8 color = {}, float scale = 1) {
            mOffset = { x, y };
            mText = text;
            mFontId = fontId;
            mColor = color;
            mScale = scale;
        }
    };
} // namespace Plutus