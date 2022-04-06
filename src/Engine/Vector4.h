#pragma once

#include "CoreMath.h"

class Vec4
{
public:
	Vec4() = default;
	Vec4(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
	Vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	Vec4(const Vec4&) = default;
	Vec4(Vec4&&) = default;
	Vec4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vec4& operator=(const Vec4&) = default;
	Vec4& operator=(Vec4&&) = default;

	int operator==(const Vec4& v) { return (fabs(x - v.x) < EpsilonF && fabs(y - v.y) < EpsilonF && fabs(z - v.z) < EpsilonF && fabs(w - v.w) < EpsilonF); }
	int operator!=(const Vec4& v) { return (fabs(x - v.x) > EpsilonF || fabs(y - v.y) > EpsilonF || fabs(z - v.z) > EpsilonF || fabs(w - v.w) > EpsilonF); }

	/*const Vec4 operator*(float f) const { return { x * f, y * f, z * f, w * f }; }
	const Vec4 operator*(const Vec4& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
	const Vec4 operator/(float f) const { return { x / f, y / f, z / f, w / f }; }
	const Vec4 operator/(const Vec4& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
	const Vec4 operator+(const Vec4& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
	const Vec4 operator-() const { return { -x, -y, -z, -w }; }
	const Vec4 operator-(const Vec4& v) const { return { x - v.x, y - v.y, z - v.z, z - v.w }; }

	Vec4& operator*=(float f) { return *this = *this * f; }
	Vec4& operator*=(const Vec4& v) { return *this = *this * v; }
	Vec4& operator/=(float f) { return *this = *this / f; }
	Vec4& operator/=(const Vec4& v) { return *this = *this / v; }
	Vec4& operator+=(const Vec4& v) { return *this = *this + v; }
	Vec4& operator-=(const Vec4& v) { return *this = *this - v; }*/

	void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	float& operator[](size_t i) { assert(i < 0 || i > 3); return (&x)[i]; }
	const float operator[](size_t i) const { assert(i < 0 || i > 3); return (&x)[i]; }


	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};