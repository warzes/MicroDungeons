#pragma once

#include "Array.h"

struct BTri
{
	void Split(BTri* dest, int& nPos, int& nNeg, const Vector4& plane, const float epsilon) const;
	void Finalize();

	bool IsAbove(const Vector3& pos) const;

	Vector4 plane;
	Vector4 edgePlanes[3];

	Vector3 v[3];

	void* data = nullptr;
};

struct BNode
{
	~BNode();

	void Build(Array<BTri>& tris, const int splitCost, const int balCost, const float epsilon);

	bool Intersects(const Vector3& v0, const Vector3& v1, const Vector3& dir, Vector3* point, const BTri** triangle) const;

	bool PushSphere(Vector3& pos, const float radius) const;

	BNode* back = nullptr;
	BNode* front = nullptr;

	BTri tri;
};

class BSP
{
public:
	~BSP();

	void AddTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, void* data = nullptr);

	void Build(const int splitCost = 3, const int balCost = 1, const float epsilon = 0.001f);

	bool Intersects(const Vector3& v0, const Vector3& v1, Vector3* point = nullptr, const BTri** triangle = nullptr) const;

	bool PushSphere(Vector3& pos, const float radius) const;

private:
	Array<BTri> m_tris;
	BNode* m_top = nullptr;
};