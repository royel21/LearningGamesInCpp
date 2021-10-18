#ifndef _VERTEX_H
#define _VERTEX_H

#include "GLheaders.h"

#include <tuple>
#include <glm/glm.hpp>
#include <Utils/Utils.h>

namespace Plutus
{
	struct Position
	{
		float x;
		float y;

		Position() : x(0), y(0) {}
		Position(float _x, float _y) : x(_x), y(_y) {}
	};

	struct UV
	{
		float u;
		float v;

		UV() : u(0), v(0) {}
		UV(float _u, float _v) : u(_u), v(_v) {}
	};

	struct ColorRGBA8
	{
		uint32_t rgba;
		//Empty Constructor
		ColorRGBA8() : rgba(0xffffffff) {}

		ColorRGBA8(uint32_t color) : rgba(color) {}

		ColorRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }

		ColorRGBA8(const glm::vec4& c) {
			rgba = RGBA2Int(float2ubyte(c.x), float2ubyte(c.y), float2ubyte(c.z), float2ubyte(c.w));
		}

		uint32_t get() { return rgba; };
		void setColor(uint32_t c) { rgba = c; }
		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }


		operator uint32_t() const { return rgba; }
		//a << 24 | b << 16 | g << 8 | r
		operator glm::vec4() const { return { (rgba & 0xff) / 255.0f, ((rgba >> 8) & 0xff) / 255.0f, ((rgba >> 16) & 0xff) / 255.0f, ((rgba >> 24) & 0xff) / 255.0f }; }
	};
	/*
		Represent a single Vertex information:
		Position Y,X
		Texture UV
		Color
	*/
	struct Vertex
	{
		Position position; /* Positon X,Y */
		UV uv;	/* Texture coord from where to sample */
		ColorRGBA8 color; /* RGBA Color for the sample */
		float entId;

		Vertex() : position(), uv() {}
		Vertex(float x, float y, float uvX, float uvY, ColorRGBA8 c, float _entId = 0) : position(x, y), uv(uvX, uvY), color(c), entId(_entId) {}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			color.setColor(r, g, b, a);
		}
	};
	//An object containing all data need for creating vertice
	struct Renderable {
		// Texture Id
		uint32_t TexId;
		// Rectangle with position x,y and width, height
		glm::vec4 trans;
		// Texture coords "UV"
		glm::vec4 uv = { 0,0,1,1 };
		// Color
		ColorRGBA8 color = {};
		// Rotation
		float r = 0;
		// Flip the texture coord on x
		bool flipX = false;
		// Flip the texture coord on y
		bool flipY = false;
		// Entity Id for mouse picking
		int entId = -1;
		// Layer Id
		int layer = 0;
		// Sort in Y order
		bool sortY = false;
		Renderable() = default;
		Renderable(uint32_t texture, glm::vec4 rect, glm::vec4 _uv, ColorRGBA8 _c) : TexId(texture), trans(rect), uv(_uv), color(_c) {}

		Renderable(uint32_t texture, glm::vec4 rect, glm::vec4 _uv, ColorRGBA8 _c, float _r, bool fx, bool fy, int id, int _layer, bool sY)
			: TexId(texture), trans(rect), uv(_uv), color(_c), r(_r), flipX(fx), flipY(fy), entId(id), layer(_layer), sortY(sY) {
		}

		bool operator < (const Renderable& rend) const {
			if (sortY && rend.sortY) {
				return std::tie(layer, rend.trans.y, TexId) < std::tie(rend.layer, trans.y, TexId);
			}
			else {
				return std::tie(layer, TexId) < std::tie(rend.layer, rend.TexId);
			}
		}
	};
} // namespace Plutus

#endif