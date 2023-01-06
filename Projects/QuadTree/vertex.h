#pragma once

#include <Graphics/Graphic.h>
namespace PlutusEngine {
	struct Position {
		float x;
		float y;
	};

	struct UV {
		float u;
		float v;
	};

	struct ColorRGBA8 {
		//Empty Constructor
		ColorRGBA8() : r(0), g(0), b(0), a(0) { }

		ColorRGBA8(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a) {
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct Vertex {
		Vertex() : position(), uv() {}
		Position position;
		ColorRGBA8 color;
		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}