#include "stdafx.h"
#include "BSP.h"

float planeDistance(const Vector3& normal, const float offset, const Vector3& point)
{
	return point.x * normal.x + point.y * normal.y + point.z * normal.z + offset;
}

float planeDistance(const Vector4& plane, const Vector3& point) {
	return point.x * plane.x + point.y * plane.y + point.z * plane.z + plane.w;
}

Vector3 planeHit(const Vector3& v0, const Vector3& v1, const Vector4& plane) {
	Vector3 dir = v1 - v0;
	float d = planeDistance(plane, v0);
	Vector3 pos = v0 - (d / Vector3DotProduct((Vector3&)plane, dir)) * dir;

	return pos;
}

void BTri::Split(BTri* dest, int& nPos, int& nNeg, const Vector4& plane, const float epsilon) const
{
	float d[3];
	for (int i = 0; i < 3; i++) {
		d[i] = planeDistance(plane, v[i]);
	}

	int first = 2;
	int second = 0;
	while (!(d[second] > epsilon && d[first] <= epsilon)) {
		first = second;
		second++;
	}

	// Positive triangles
	nPos = 0;
	Vector3 h = planeHit(v[first], v[second], plane);
	do {
		first = second;
		second++;
		if (second >= 3) second = 0;

		dest->v[0] = h;
		dest->v[1] = v[first];
		if (d[second] > epsilon) {
			dest->v[2] = v[second];
		}
		else {
			dest->v[2] = h = planeHit(v[first], v[second], plane);
		}

		dest->data = data;
		dest->Finalize();
		dest++;
		nPos++;
	} while (d[second] > epsilon);

	// Skip zero area triangle
	if (fabsf(d[second]) <= epsilon) {
		first = second;
		second++;
		if (second >= 3) second = 0;
	}

	// Negative triangles
	nNeg = 0;
	do {
		first = second;
		second++;
		if (second >= 3) second = 0;

		dest->v[0] = h;
		dest->v[1] = v[first];
		if (d[second] < -epsilon) {
			dest->v[2] = v[second];
		}
		else {
			dest->v[2] = planeHit(v[first], v[second], plane);
		}

		dest->data = data;
		dest->Finalize();
		dest++;
		nNeg++;
	} while (d[second] < -epsilon);
}

void BTri::Finalize()
{
	Vector3 normal = Vector3Normalize(Vector3CrossProduct(v[1] - v[0], v[2] - v[0]));
	float offset = -Vector3DotProduct(v[0], normal);

	Vector3 edgeNormals[3];
	edgeNormals[0] = Vector3CrossProduct(normal, v[0] - v[2]);
	edgeNormals[1] = Vector3CrossProduct(normal, v[1] - v[0]);
	edgeNormals[2] = Vector3CrossProduct(normal, v[2] - v[1]);

	float edgeOffsets[3];
	edgeOffsets[0] = Vector3DotProduct(edgeNormals[0], v[0]);
	edgeOffsets[1] = Vector3DotProduct(edgeNormals[1], v[1]);
	edgeOffsets[2] = Vector3DotProduct(edgeNormals[2], v[2]);

	plane = Vector4(normal.x, normal.y, normal.z, offset);
	edgePlanes[0] = Vector4(edgeNormals[0].x, edgeNormals[0].y, edgeNormals[0].z, -edgeOffsets[0]);
	edgePlanes[1] = Vector4(edgeNormals[1].x, edgeNormals[1].y, edgeNormals[1].z, -edgeOffsets[1]);
	edgePlanes[2] = Vector4(edgeNormals[2].x, edgeNormals[2].y, edgeNormals[2].z, -edgeOffsets[2]);
}

bool BTri::IsAbove(const Vector3& pos) const
{
	return (planeDistance(edgePlanes[0], pos) >= 0 && planeDistance(edgePlanes[1], pos) >= 0 && planeDistance(edgePlanes[2], pos) >= 0);
}

BNode::~BNode()
{
	delete back;
	delete front;
}

void BNode::Build(Array<BTri>& tris, const int splitCost, const int balCost, const float epsilon)
{
	uint32_t index = 0;
	int minScore = 0x7FFFFFFF;

	for (uint32_t i = 0; i < tris.getCount(); i++) {
		int score = 0;
		int diff = 0;
		for (uint32_t k = 0; k < tris.getCount(); k++) {
			uint32_t neg = 0, pos = 0;
			for (uint32_t j = 0; j < 3; j++) {
				float dist = planeDistance(tris[i].plane, tris[k].v[j]);
				if (dist < -epsilon) neg++; else
					if (dist > epsilon) pos++;
			}
			if (pos) {
				if (neg) score += splitCost; else diff++;
			}
			else {
				if (neg) diff--; else diff++;
			}
		}
		score += balCost * abs(diff);
		if (score < minScore) 
		{
			minScore = score;
			index = i;
		}
	}

	tri = tris[index];
	tris.fastRemove(index);

	Array <BTri> backTris;
	Array <BTri> frontTris;
	for (uint32_t i = 0; i < tris.getCount(); i++) 
	{

		uint32_t neg = 0, pos = 0;
		for (uint32_t j = 0; j < 3; j++) 
		{
			float dist = planeDistance(tri.plane, tris[i].v[j]);
			if (dist < -epsilon) neg++; else
				if (dist > epsilon) pos++;
		}

		if (neg)
		{
			if (pos) 
			{
				BTri newTris[3];
				int nPos, nNeg;
				tris[i].Split(newTris, nPos, nNeg, tri.plane, epsilon);
				for (int i = 0; i < nPos; i++)
				{
					frontTris.add(newTris[i]);
				}
				for (int i = 0; i < nNeg; i++) 
				{
					backTris.add(newTris[nPos + i]);
				}
			}
			else 
			{
				backTris.add(tris[i]);
			}
		}
		else 
		{
			frontTris.add(tris[i]);
		}
	}
	tris.reset();

	if (backTris.getCount() > 0) 
	{
		back = new BNode;
		back->Build(backTris, splitCost, balCost, epsilon);
	}
	else back = nullptr;

	if (frontTris.getCount() > 0) 
	{
		front = new BNode;
		front->Build(frontTris, splitCost, balCost, epsilon);
	}
	else front = nullptr;
}

bool BNode::Intersects(const Vector3& v0, const Vector3& v1, const Vector3& dir, Vector3* point, const BTri** triangle) const
{
#if 0
	float d0 = planeDistance(tri.plane, v0);
	float d1 = planeDistance(tri.plane, v1);

	vec3 pos;
	if (d0 > 0) {
		if (d1 <= 0) {
			pos = v0 - (d0 / dot(tri.plane.xyz(), dir)) * dir;
		}

		if (front != NULL && front->intersects(v0, (d1 <= 0) ? pos : v1, dir, point, triangle)) return true;

		if (d1 <= 0) {
			if (tri.isAbove(pos)) {
				if (point) *point = pos;
				if (triangle) *triangle = &tri;
				return true;
			}
			if (back != NULL && back->intersects(pos, v1, dir, point, triangle)) return true;
		}
	}
	else {
		if (d1 > 0) {
			pos = v0 - (d0 / dot(tri.plane.xyz(), dir)) * dir;
		}

		if (back != NULL && back->intersects(v0, (d1 > 0) ? pos : v1, dir, point, triangle)) return true;

		if (d1 > 0) {
			if (tri.isAbove(pos)) {
				if (point) *point = pos;
				if (triangle) *triangle = &tri;
				return true;
			}
			if (front != NULL && front->intersects(pos, v1, dir, point, triangle)) return true;
		}
	}

#else
	float d = planeDistance(tri.plane, v0);

	if (d > 0) {
		if (front != nullptr && front->Intersects(v0, v1, dir, point, triangle)) return true;
		if (planeDistance(tri.plane, v1) < 0) 
		{
			Vector3 pos = v0 - (d / Vector3DotProduct({ tri.plane.x, tri.plane.y, tri.plane.z }, dir)) * dir;
			if (tri.IsAbove(pos)) 
			{
				if (point) *point = pos;
				if (triangle) *triangle = &tri;
				return true;
			}
			if (back != nullptr && back->Intersects(v0, v1, dir, point, triangle)) return true;
		}
	}
	else {
		if (back != nullptr && back->Intersects(v0, v1, dir, point, triangle)) return true;
		if (planeDistance(tri.plane, v1) > 0) 
		{
			Vector3 pos = v0 - (d / Vector3DotProduct({ tri.plane.x, tri.plane.y, tri.plane.z }, dir)) * dir;
			if (tri.IsAbove(pos))
			{
				if (point) *point = pos;
				if (triangle) *triangle = &tri;
				return true;
			}
			if (front != nullptr && front->Intersects(v0, v1, dir, point, triangle)) return true;
		}
	}
#endif

	return false;
}

bool BNode::PushSphere(Vector3& pos, const float radius) const
{
	float d = planeDistance(tri.plane, pos);

	bool pushed = false;
	if (fabsf(d) < radius) 
	{
		if (tri.IsAbove(pos)) 
		{
			//			pos += (radius - d) * tri.normal;
			Vector3 t = (radius - d) * Vector3(tri.plane.x, tri.plane.y, tri.plane.z);
			pos.x += t.x;
			pos.y += t.y;
			pos.z += t.z;
			pushed = true;
		}
	}

	if (front != nullptr && d > -radius) pushed |= front->PushSphere(pos, radius);
	if (back != nullptr && d < radius) pushed |= back->PushSphere(pos, radius);

	return pushed;
}

BSP::~BSP()
{
	delete m_top;
}

void BSP::AddTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, void* data)
{
	BTri tri;
	tri.v[0] = v0;
	tri.v[1] = v1;
	tri.v[2] = v2;
	tri.data = data;
	tri.Finalize();

	m_tris.add(tri);
}

void BSP::Build(const int splitCost, const int balCost, const float epsilon)
{
	m_top = new BNode;
	m_top->Build(m_tris, splitCost, balCost, epsilon);
}

bool BSP::Intersects(const Vector3& v0, const Vector3& v1, Vector3* point, const BTri** triangle) const
{
	if (m_top != nullptr) return m_top->Intersects(v0, v1, v1 - v0, point, triangle);
	return false;
}

bool BSP::PushSphere(Vector3& pos, const float radius) const
{
	if (m_top != nullptr) return m_top->PushSphere(pos, radius);
	return false;
}