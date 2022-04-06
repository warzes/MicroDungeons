#pragma once

#include "CoreMath.h"

class Vec3
{
public:
	Vec3() = default;
	Vec3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}
	Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
	Vec3(const Vec3&) = default;
	Vec3(Vec3&&) = default;
	Vec3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

	Vec3& operator=(const Vec3&) = default;
	Vec3& operator=(Vec3&&) = default;

	int operator==(const Vec3& v) { return (fabs(x - v.x) < EpsilonF && fabs(y - v.y) < EpsilonF && fabs(z - v.z) < EpsilonF); }
	int operator!=(const Vec3& v) { return (fabs(x - v.x) > EpsilonF || fabs(y - v.y) > EpsilonF || fabs(z - v.z) > EpsilonF); }

	const Vec3 operator*(float f) const { return { x * f, y * f, z * f }; }
	const Vec3 operator*(const Vec3& rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
	const Vec3 operator/(float f) const { return { x / f, y / f, z / f }; }
	const Vec3 operator/(const Vec3& rhs) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
	const Vec3 operator+(const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
	const Vec3 operator-() const { return { -x, -y, -z }; }
	const Vec3 operator-(const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }

	Vec3& operator*=(float f) { return *this = *this * f; }
	Vec3& operator*=(const Vec3& v) { return *this = *this * v; }
	Vec3& operator/=(float f) { return *this = *this / f; }
	Vec3& operator/=(const Vec3& v) { return *this = *this / v; }
	Vec3& operator+=(const Vec3& v) { return *this = *this + v; }
	Vec3& operator-=(const Vec3& v) { return *this = *this - v; }

	void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; }

	operator float* () { return &x; }
	operator const float* () const { return &x; }

	float& operator[](size_t i) { assert(i < 0 || i > 2); return (&x)[i]; }
	const float operator[](size_t i) const { assert(i < 0 || i > 2); return (&x)[i]; }

	// TODO: возможно ненужное
	float DotProduct(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
	static float DotProduct(const Vec3& v1, const Vec3& v2) { return v1.DotProduct(v2); }

	Vec3 CrossProduct(const Vec3& v) const { return { y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y }; }
	static Vec3 CrossProduct(const Vec3& v1, const Vec3& v2) { return v1.CrossProduct(v2); }

	float LengthSquared() const { return x * x + y * y + z * z; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	float Distance(const Vec3& v) const { const Vec3 diff(x - v.x, y - v.y, z - v.z); return diff.Length(); }
	const Vec3 Normalized() const { const float s = Length(); return { x / s, y / s, z / s }; }


	float x = 0;
	float y = 0;
	float z = 0;
};