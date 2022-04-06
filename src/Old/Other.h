#pragma once

struct Point2
{
	int x;
	int y;
};

inline Point2 operator-(const Point2& a, const Point2& b)
{
	return { a.x - b.x, a.y - b.y };
}

inline bool operator==(const Point2& a, const Point2& b)
{
	if ((a.x == b.x) && (a.y == b.y)) return true;
	return false;
}

inline Vector3 Mix(const Vector3& x, const Vector3& y, float a)
{
	return (x * (1.0f - a) + y * a);
}