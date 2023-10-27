#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace dp {
	
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	
	inline ew::Mat4 Scale(const ew::Vec3& s) {
		return ew::Mat4(
			s.x, 0.0f, 0.0f, 0.0f,
			0.0f, s.y, 0.0f, 0.0f,
			0.0f, 0.0f, s.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	
	inline ew::Mat4 RotateX(float rad) {
		return ew::Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosf(rad), -sinf(rad), 0.0f,
			0.0f, sinf(rad), cosf(rad), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	
	inline ew::Mat4 RotateY(float rad) {
		return ew::Mat4(
			cosf(rad), 0.0f, sinf(rad), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinf(rad), 0.0f, cosf(rad), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	
	inline ew::Mat4 RotateZ(float rad) {
		return ew::Mat4(
			cosf(rad), -sinf(rad), 0.0f, 0.0f,
			sinf(rad), cosf(rad), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(const ew::Vec3& t) {
		return ew::Mat4(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	inline ew::Mat4 LookAt(const ew::Vec3& e, const ew::Vec3& t, const ew::Vec3& up) {
		ew::Vec3 f = ew::Normalize(e - t);
		ew::Vec3 r = ew::Normalize(ew::Cross(up, f));
		ew::Vec3 u = ew::Normalize(ew::Cross(f, r));

		return ew::Mat4(
			r.x, r.y, r.z, -ew::Dot(r, e),
			u.x, u.y, u.z, -ew::Dot(u, e),
			f.x, f.y, f.z, -ew::Dot(f, e),
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline ew::Mat4 Perspective(float fov, float a, float n, float f) {
		float c = tanf(fov / 2.0f);

		ew::Mat4 m = ew::Mat4(0);
		m[0][0] = 1.0f / (c * a);
		m[1][1] = 1.0f / c;
		m[2][2] = (f + n) / (n - f);
		m[3][2] = (2.0f * f * n) / (n - f);
		m[2][3] = -1.0f;
		return m;
	}

	inline ew::Mat4 Orthographic(float h, float a, float n, float f) {
		float t = h / 2.0f;
		float b = -t;
		float r = (h * a) / 2.0f;
		float l = -r;

		ew::Mat4 m = ew::Mat4(0);
		m[0][0] = 2.0f / (r - l);
		m[1][1] = 2.0f / (t - b);
		m[2][2] = -2.0f / (f - n);
		m[3][0] = -(r + l) / (r - l);
		m[3][1] = -(t + b) / (t - b);
		m[3][2] = -(f + n) / (f - n);
		m[3][3] = 1.0f;
		return m;
	}
}