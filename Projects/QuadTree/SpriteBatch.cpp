#include "SpriteBatch.h"
#include <algorithm>
#include <iostream>
namespace PlutusEngine {


	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0), _sortType(PlutusEngine::GlyphSortType::TEXTURE)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
		if (_vbo)
			glDeleteBuffers(1, &_vbo);
		if (_vao)
			glDeleteVertexArrays(1, &_vao);
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::setActiveTexture(uint8_t slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
	}

	void SpriteBatch::begin(GlyphSortType sortType)
	{
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		_glyphPonters.resize(_glyphs.size());
		for (size_t i = 0; i < _glyphs.size(); i++)
		{
			_glyphPonters[i] = &_glyphs[i];
		}

		sortGlyph();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRec, const glm::vec4& uvRect, const GLuint texture, float depth, ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRec, uvRect, texture, depth, color);
	}

	void SpriteBatch::submit(const glm::vec4& destRec, ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRec, glm::vec4(0, 0, 1, 1), 0, 0, color);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for (size_t i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);

		if (_glyphs.empty()) return;

		int offset = 0;
		int cv = 0; //Current Vertex
		//Create and add a new RenderBatch
		_renderBatches.emplace_back(offset, 6, _glyphPonters[0]->texture);

		vertices[cv++] = _glyphPonters[0]->topLeft;
		vertices[cv++] = _glyphPonters[0]->bottomLeft;
		vertices[cv++] = _glyphPonters[0]->bottomRight;
		vertices[cv++] = _glyphPonters[0]->bottomRight;
		vertices[cv++] = _glyphPonters[0]->topRight;
		vertices[cv++] = _glyphPonters[0]->topLeft;

		offset += 6;

		for (size_t cg = 1; cg < _glyphPonters.size(); cg++) {
			if (_glyphPonters[cg]->texture != _glyphPonters[cg - 1]->texture) {

				_renderBatches.emplace_back(offset, 6, _glyphPonters[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = _glyphPonters[cg]->topLeft;
			vertices[cv++] = _glyphPonters[cg]->bottomLeft;
			vertices[cv++] = _glyphPonters[cg]->bottomRight;
			vertices[cv++] = _glyphPonters[cg]->bottomRight;
			vertices[cv++] = _glyphPonters[cg]->topRight;
			vertices[cv++] = _glyphPonters[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//Orphan the buffer first
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		//Create the Vertex Array Buffer
		if (!_vao) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		//Create the Vertex Array Buffer
		if (!_vbo) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		/* Tell OpenGL that we want to use vertexPosition and its location is 0*/
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		/* Tell OpenGL that we want to use vertexColor and its location is1*/
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		/* Tell OpenGL that we want to use vertexUV and its location is 2*/
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyph()
	{
		switch (_sortType)
		{
		case PlutusEngine::GlyphSortType::NONE:
			break;
		case PlutusEngine::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPonters.begin(), _glyphPonters.end(), compareFrontToBack);
			break;
		case PlutusEngine::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPonters.begin(), _glyphPonters.end(), compareBackToFront);
			break;
		case PlutusEngine::GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPonters.begin(), _glyphPonters.end(), compareTexture);
			break;
		default:
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}