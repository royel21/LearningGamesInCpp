#ifndef _TEXTLAYER_H
#define _TEXTLAYER_H

#include <map>

#include <iostream>
#include <glm/glm.hpp>

#include "Graphics/GLheaders.h"
#include "Graphics/Shader.h"

namespace Plutus
{

	class Shader;
	class Window;

	struct TextCharacter
	{
		GLuint TextureID;	// ID handle of the glyph texture
		glm::ivec2 Size;	// Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		GLuint Advance;		// Horizontal offset to advance to next glyph
		TextCharacter() : TextureID(0), Size(1), Bearing(1), Advance(0) {};
		TextCharacter(GLuint texid, glm::ivec2 sz, glm::ivec2 bring, GLuint adv) : TextureID(texid), Size(sz), Bearing(bring), Advance(adv) {};
	};

	class TextLayer
	{
	private:
		GLuint pVAO = 0;
		GLuint pVBO = 0;
		GLushort mActiveTexture = 0;
		float pWidth = 0.0f;
		float pHeight = 0.0f;
		glm::vec4 color;
		std::map<char, TextCharacter> pCharset;
		Shader pShader;

	public:
		TextLayer() = default;
		~TextLayer();

		bool Init(float width, float height, std::string fontPath, uint32_t fontSize);

		inline void setActiveTexture(GLushort slot)
		{
			mActiveTexture = slot;
		}

		inline void setColor(float r, float g, float b, float a = 1) { color = glm::vec4(r, g, b, a); };

		void drawString(std::string text, GLfloat x, GLfloat y, GLfloat scale = 1.0f, glm::vec4 color = { 0, 0, 0, 1 });
		void updateCamera(int screenWidth, int screenHeight);

	private:
		void initFreetypeFont(std::string fontPath, uint32_t fontSize);
	};
} // namespace Plutus

#endif