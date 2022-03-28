#pragma once

#include <cassert>
#include <raylib/raylib.h>
#include <raylib/raymath.h>

// ref: https://github.com/ProfJski/RaylibOpOverloads

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return { a.x + b.x, a.y + b.y };
}

inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vector4 operator+(const Vector4& a, const Vector4& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline Vector2 operator-(const Vector2& a, const Vector2& b) 
{
	return { a.x - b.x, a.y - b.y };
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector4 operator-(const Vector4& a, const Vector4& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline Vector2 operator*(const Vector2& v, float scale)
{
	return { v.x * scale, v.y * scale };
}

inline Vector3 operator*(const Vector3& v, float scalar)
{
	return { v.x * scalar, v.y * scalar, v.z * scalar };
}

inline Vector3 operator*(const float s, const Vector3& v) 
{
	return { v.x * s, v.y * s, v.z * s };
}

inline Vector4 operator*(const Vector4& v, float scalar)
{
	return { v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar };
}

inline Vector4 operator*(const Vector4& a, const Vector4& b)
{
	return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline Vector2 operator/(const Vector2& a, const float b) 
{
	assert(b != 0.0 && "Division by zero error.");
	const float recip = 1.0 / b;
	return a * recip;
}

inline Vector3 operator/(const Vector3& a, const float b) 
{
	assert(b != 0.0 && "Division by zero error.");
	const float recip = 1.0 / b;
	return a * recip;
}

inline Vector3 operator/(const float s, const Vector3& v)
{
	return Vector3(s / v.x, s / v.y, s / v.z);
}