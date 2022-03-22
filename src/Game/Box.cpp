#include "stdafx.h"
#include "Box.h"
#include "mytritri.h"

int tri_tri_intersect(float V0[3], float V1[3], float V2[3], float U0[3], float U1[3], float U2[3]);

namespace Internal {

	////////////////////////////////////////////////////
	// code from here is used in detection process

	std::size_t BoxTreeInnerNode::getTrianglesNumber() const
	{
		return m_Boxes.size();
	}

	const BoxedTriangle* BoxTreeInnerNode::getTriangle(std::size_t which) const
	{
		if (which >= getTrianglesNumber()) return nullptr;
		return m_Boxes[which];
	}

	RotationState::RotationState(const Matrix3D& transform)
		: t(transform)
	{
		N[0] = Vector3D(t(0, 0), t(0, 1), t(0, 2));
		N[1] = Vector3D(t(1, 0), t(1, 1), t(1, 2));
		N[2] = Vector3D(t(2, 0), t(2, 1), t(2, 2));
	}

	inline float DotWithCol(const Vector3D& v, const Matrix3& m, int col)
	{
		return v.x * m(0, col) + v.y * m(1, col) + v.z * m(2, col);
	}

	bool Box::intersect(const Vector3D& O, float radius) const
	{
		Vector3D mx = m_Pos + m_Size;
		float dist = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			if (O[i] < m_Pos[i])
			{
				float d = O[i] - m_Pos[i];
				dist += d * d;
			}
			else
				if (O[i] > mx[i])
				{
					float d = O[i] - mx[i];
					dist += d * d;
				}
		}
		return (dist <= (radius * radius));
	}

	bool Box::intersect(const Vector3D& O, const Vector3D& D, float segmax) const
	{
		if (segmax > 3e30f) return intersect(O, D); // infinite ray
		Vector3D abs_segdir, abs_diff, abs_cross;

		Vector3D segdir = 0.5f * segmax * D;
		Vector3D seg_center = O + segdir;
		Vector3D diff = seg_center - getCenter();
		int i;
		for (i = 0; i < 3; i++)
		{
			abs_segdir[i] = flabs(segdir[i]);
			abs_diff[i] = flabs(diff[i]);
			float f = getSize()[i] + abs_segdir[i];
			if (abs_diff[i] > f) return false;
		}
		Vector3D cross = CrossProduct(segdir, diff);
		int idx[] = { 0,1,2,0,1 };
		for (i = 0; i < 3; i++)
		{
			int i1 = idx[i + 1];
			int i2 = idx[i + 2];
			abs_cross[i] = flabs(cross[i]);
			float f = getSize()[i1] * abs_segdir[i2] + getSize()[i2] * abs_segdir[i1];
			if (abs_cross[i] > f) return false;
		}
		return true;
	}

	bool Box::intersect(const Vector3D& O, const Vector3D& D) const
	{
		Vector3D abs_segdir, abs_cross;
		float f;
		Vector3D diff = O - getCenter();

		for (int i = 0; i < 3; i++)
		{
			abs_segdir[i] = flabs(D[i]);
			if (flabs(diff[i]) > m_Size[i] && diff[i] * D[i] >= 0.0f)
				return false;
		}

		Vector3D cross = CrossProduct(D, diff);

		abs_cross[0] = flabs(cross[0]);
		f = m_Size[1] * abs_segdir[2] + m_Size[2] * abs_segdir[1];
		if (abs_cross[0] > f)
			return false;

		abs_cross[1] = flabs(cross[1]);
		f = m_Size[0] * abs_segdir[2] + m_Size[2] * abs_segdir[0];
		if (abs_cross[1] > f)
			return false;

		abs_cross[2] = flabs(cross[2]);
		f = m_Size[0] * abs_segdir[1] + m_Size[1] * abs_segdir[0];
		if (abs_cross[2] > f)
			return false;

		return true;
	}

	bool Box::intersect(const Box& b, RotationState& rs) const
	{
		const Vector3D bCenter = Transform(b.getCenter(), rs.t);
		Vector3D EA = 0.5f * getSize();
		Vector3D EB = 0.5f * b.getSize();
		Vector3D distance = bCenter - getCenter();
		Matrix3 C, abs_C;
		float   R0, R1, R, R01;
		int i;

		for (i = 0; i < 3; i++)
		{
			C(i, 0) = rs.N[0][i];
			C(i, 1) = rs.N[1][i];
			C(i, 2) = rs.N[2][i];
			abs_C(i, 0) = flabs(C(i, 0));
			abs_C(i, 1) = flabs(C(i, 1));
			abs_C(i, 2) = flabs(C(i, 2));
			R = flabs(distance[i]);
			R1 = EB * abs_C.baseRow(i);
			R01 = EA[i] + R1;
			if (R > R01) return false;
		}
		for (i = 0; i < 3; i++)
		{
			R = flabs(rs.N[i] * distance);
			R0 = DotWithCol(EA, abs_C, i);
			R01 = R0 + EB[i];
			if (R > R01) return false;
		}

		R = flabs(distance.z * C(1, 0) - distance.y * C(2, 0));
		R0 = EA.y * abs_C(2, 0) + EA.z * abs_C(1, 0);
		R1 = EB.y * abs_C(0, 2) + EB.z * abs_C(0, 1);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.z * C(1, 1) - distance.y * C(2, 1));
		R0 = EA.y * abs_C(2, 1) + EA.z * abs_C(1, 1);
		R1 = EB.x * abs_C(0, 2) + EB.z * abs_C(0, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.z * C(1, 2) - distance.y * C(2, 2));
		R0 = EA.y * abs_C(2, 2) + EA.z * abs_C(1, 2);
		R1 = EB.x * abs_C(0, 1) + EB.y * abs_C(0, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.x * C(2, 0) - distance.z * C(0, 0));
		R0 = EA.x * abs_C(2, 0) + EA.z * abs_C(0, 0);
		R1 = EB.y * abs_C(1, 2) + EB.z * abs_C(1, 1);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.x * C(2, 1) - distance.z * C(0, 1));
		R0 = EA.x * abs_C(2, 1) + EA.z * abs_C(0, 1);
		R1 = EB.x * abs_C(1, 2) + EB.z * abs_C(1, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.x * C(2, 2) - distance.z * C(0, 2));
		R0 = EA.x * abs_C(2, 2) + EA.z * abs_C(0, 2);
		R1 = EB.x * abs_C(1, 1) + EB.y * abs_C(1, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.y * C(0, 0) - distance.x * C(1, 0));
		R0 = EA.x * abs_C(1, 0) + EA.y * abs_C(0, 0);
		R1 = EB.y * abs_C(2, 2) + EB.z * abs_C(2, 1);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.y * C(0, 1) - distance.x * C(1, 1));
		R0 = EA.x * abs_C(1, 1) + EA.y * abs_C(0, 1);
		R1 = EB.x * abs_C(2, 2) + EB.z * abs_C(2, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		R = flabs(distance.y * C(0, 2) - distance.x * C(1, 2));
		R0 = EA.x * abs_C(1, 2) + EA.y * abs_C(0, 2);
		R1 = EB.x * abs_C(2, 1) + EB.y * abs_C(2, 0);
		R01 = R0 + R1;
		if (R > R01) return false;

		return true;
	}

	Triangle::Triangle(const Vector3D& _1, const Vector3D& _2, const Vector3D& _3)
		: v1(_1), v2(_2), v3(_3), center((1.0f / 3.0f)* (_1 + _2 + _3))
	{}

	bool Triangle::intersect(const Vector3D& O, const Vector3D& D, Vector3D& cp,
		float& tparm, float segmax) const
	{
		Plane p(v1, v2, v3);
		float denom = p.normal * D;
		if (fuzzyIsNull(denom)) return false;
		float t = -(p.d + p.normal * O) / denom;
		if (t <= 0.0f) return false;
		if (t > segmax) return false;
		TriangleDesc td(*this, p);
		cp = O + t * D;
		if (td.pointInTri(cp))
		{
			tparm = t;
			return true;
		}
		return false;
	}

	bool Triangle::intersect(const Vector3D& O, float radius, Vector3D* cp) const
	{
		if (cp == nullptr)
			return false;

		Vector3D& rcp = *cp;
		const Plane p(this->v1, this->v2, this->v3);
		const float dist = p.Classify(O);
		if (flabs(dist) > radius)
			return false;
		Vector3D point = O - dist * p.normal;
		TriangleDesc td(*this, p);
		if (td.pointInTri(point)) {
			rcp = point;
			return true;
		}
		/////////////////////////////////////////////////////////
		// Added code for edge intersection detection
		const Vector3D* v[] = { &this->v1, &this->v2, &this->v3, &this->v1 };
		for (int i = 0; i < 3; ++i) {
			const Vector3D u(*v[i + 1] - *v[i]), pa(O - *v[i]);
			const float s = (u * pa) / u.SquareMagnitude();
			if (s < 0)
				rcp = *v[i];
			else if (s > 1)
				rcp = *v[i + 1];
			else
				rcp = *v[i] + s * u;
			const float sq_dist = (O - rcp).SquareMagnitude();
			if (sq_dist < (radius * radius))
				return true;
		}
		/////////////////////////////////////////////////////////
		return false;
	}

	void Triangle::copyCoords(float array[]) const
	{
		if (array != nullptr) {
			*((Vector3D*)&array[0]) = this->v1;
			*((Vector3D*)&array[3]) = this->v2;
			*((Vector3D*)&array[6]) = this->v3;
		}
	}

	bool Triangle::intersect(const Triangle& t) const
	{
		return (tri_tri_intersect((float*)&v1.x,
			(float*)&v2.x,
			(float*)&v3.x,
			(float*)&t.v1.x,
			(float*)&t.v2.x,
			(float*)&t.v3.x) != 0);
	}

} // namespace Internal