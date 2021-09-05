#ifndef _VERTEX_H
#define _VERTEX_H

#include <glm/glm.hpp>
#include "GLheaders.h"

namespace Plutus
{
	struct Position
	{
		Position() : x(0), y(0) {}
		Position(float _x, float _y) : x(_x), y(_y) {}
		float x;
		float y;
	};

	struct UV
	{
		UV() : u(0), v(0) {}
		UV(float _u, float _v) : u(_u), v(_v) {}
		float u;
		float v;
	};

	struct ColorRGBA8
	{
		GLuint rgba;
		//Empty Constructor
		ColorRGBA8() : rgba(0xffffffff) {}

		ColorRGBA8(uint32_t color) : rgba(color) {}

		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255)
		{
			rgba = a << 24 | b << 16 | g << 8 | r;
		}
		GLuint get() { return rgba; };
		void setColor(GLuint c) { rgba = c; }
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255) { rgba = a << 24 | b << 16 | g << 8 | r; };
		operator GLuint() const { return rgba; }
	};
	/*
		Represent a single Vertex information
	*/
	struct Vertex
	{
		Position position; /* Positon X,Y */
		UV uv;	/* Texture coord from where to sample */
		ColorRGBA8 color; /* RGBA Color for the sample */

		Vertex() : position(), uv() {}
		Vertex(float x, float y, float uvX, float uvY, ColorRGBA8 c) : position(x, y), uv(uvX, uvY), color(c) {}

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

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.setColor(r, g, b, a);
		}
	};

	struct Renderable {
		GLuint TexId;
		glm::vec4 trans;
		glm::vec4 uv = { 0,0,1,1 };
		ColorRGBA8 color = {};
		float r = 0;
		bool flipX = false;
		bool flipY = false;
	};
} // namespace Plutus

#endif