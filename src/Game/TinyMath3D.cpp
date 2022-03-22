#include "stdafx.h"
#include "TinyMath3D.h"

namespace Internal {

	const Vector3D Vector3D::Zero(0.0f, 0.0f, 0.0f);
	const Matrix3D Matrix3D::Identity(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);


	inline float
		MINOR(const Matrix3D& m, const int r0, const int r1, const int r2, const int c0, const int c1, const int c2)
	{
		return m(r0, c0) * (m(r1, c1) * m(r2, c2) - m(r2, c1) * m(r1, c2)) -
			m(r0, c1) * (m(r1, c0) * m(r2, c2) - m(r2, c0) * m(r1, c2)) +
			m(r0, c2) * (m(r1, c0) * m(r2, c1) - m(r2, c0) * m(r1, c1));
	}


	Matrix3D
		Matrix3D::Adjoint() const
	{
		return Matrix3D(MINOR(*this, 1, 2, 3, 1, 2, 3),
			-MINOR(*this, 0, 2, 3, 1, 2, 3),
			MINOR(*this, 0, 1, 3, 1, 2, 3),
			-MINOR(*this, 0, 1, 2, 1, 2, 3),

			-MINOR(*this, 1, 2, 3, 0, 2, 3),
			MINOR(*this, 0, 2, 3, 0, 2, 3),
			-MINOR(*this, 0, 1, 3, 0, 2, 3),
			MINOR(*this, 0, 1, 2, 0, 2, 3),

			MINOR(*this, 1, 2, 3, 0, 1, 3),
			-MINOR(*this, 0, 2, 3, 0, 1, 3),
			MINOR(*this, 0, 1, 3, 0, 1, 3),
			-MINOR(*this, 0, 1, 2, 0, 1, 3),

			-MINOR(*this, 1, 2, 3, 0, 1, 2),
			MINOR(*this, 0, 2, 3, 0, 1, 2),
			-MINOR(*this, 0, 1, 3, 0, 1, 2),
			MINOR(*this, 0, 1, 2, 0, 1, 2));
	}


	float
		Matrix3D::Determinant() const
	{
		return m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
			m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
			m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
			m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
	}

	Matrix3D
		Matrix3D::Inverse() const
	{
		return (1.0f / Determinant()) * Adjoint();
	}



	Vector3D* Vector3D::asPointer(float* floatPtr)
	{
		return (Vector3D*)floatPtr;
	}

	const Vector3D* Vector3D::asConstPointer(const float* floatPtr)
	{
		return (const Vector3D*)floatPtr;
	}

	Vector3D& Vector3D::asRef(float* floatPtr)
	{
		return *(asPointer(floatPtr));
	}

	const Vector3D& Vector3D::asConstRef(const float* floatPtr)
	{
		return *(asConstPointer(floatPtr));
	}

	void Vector3D::fillFloatVec(float vec[], float value)
	{
		std::fill(vec, vec + 3, value);
	}

	void Vector3D::setFloatVec(float vec[], float x, float y, float z)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}

} // namespace Internal