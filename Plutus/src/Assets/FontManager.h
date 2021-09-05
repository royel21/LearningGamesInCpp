#ifndef _TEXTLAYER_H
#define _TEXTLAYER_H

#include <Graphics/vertex.h>
#include <unordered_map>
#include <string>

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;

namespace Plutus
{
	class Shader;
	class Window;
	struct CharData {
		float ax; // advance x distance for the next letter
		float ay; // advance y distance for the next letter

		float bw; // width of the letter in pixel ;
		float bh; // height of the letter in pixel;

		float bl; // bitmap left;
		float bt; // bitmap top;

		glm::vec4 uv; // x offset of glyph in texture coordinates
	};

	struct FontTexture {
		u32 width;
		u32 height;
		CharData ch[128];
		u32 textId;
	};

	class FontManager
	{
	private:
		ColorRGBA8 color;
		GLushort mActiveTexture = 0;
		std::string mCurrentFont;
		std::unordered_map<std::string, FontTexture> mFonts;

	public:
		//Return instace of Font Manager
		static FontManager* get();
		~FontManager();
		bool addFont(const std::string& Id, const std::string& fontPath, u32 fontSize);
		void setFont(const std::string& Id) { mCurrentFont = Id; }
		FontTexture* getFont(const std::string& Id) { return &mFonts[Id]; };

		std::vector<Renderable> renderText(const std::string& text, float x, float y, float scale = 1.0f, ColorRGBA8 color = {});

		inline void setColor(u8 r, u8 g, u8 b, u8 a) { color = { r,g,b,a }; };

	private:
		FontManager() = default;
	};
} // namespace Plutus

#endif