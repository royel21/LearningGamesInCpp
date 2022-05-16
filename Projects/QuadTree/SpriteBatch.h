#pragma once
#include <Graphics/Graphic.h>
#include <glm/glm.hpp>
#include "vertex.h"
#include <vector>

namespace PlutusEngine {
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph {
	public:
		Glyph() : texture(), depth(0), topLeft(), bottomLeft(), bottomRight(), topRight() {};
		Glyph(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint _texture, float _depth, ColorRGBA8& color) {
			texture = _texture;
			depth = _depth;

			topLeft.color = color;
			topLeft.setPosition(destRec.x, destRec.y + destRec.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRec.x, destRec.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRec.x + destRec.z, destRec.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRec.x + destRec.z, destRec.y + destRec.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex bottomRight;
		Vertex topRight;
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset), numVertices(NumVertices), texture(Texture) {
		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();
		void setActiveTexture(uint8_t slot);
		//Default Sort Type TEXTURE
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

		void end();

		void draw(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint texture, float depth, ColorRGBA8& color);
		void submit(const glm::vec4& destRec, ColorRGBA8& color);

		void renderBatch();
	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyph();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;

		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphPonters; //For sorting
		std::vector<Glyph> _glyphs;       //Those are the actual glyph
		std::vector<RenderBatch> _renderBatches;
	};

}