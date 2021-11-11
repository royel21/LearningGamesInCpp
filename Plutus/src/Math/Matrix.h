#pragma once
#include "Vectors.h"

namespace Plutus {
	class Mat4 {

	public:
		Mat4();

	private:
		float v[4][4];
	};

	Mat4 scale(const Mat4& m, const vec3f& v);
	Mat4 rotate(const Mat4& m, const vec3f& v);
	Mat4 translate(const Mat4& m, const vec3f& v);
	Mat4 ortho(float x, float y, float w, float h);

}