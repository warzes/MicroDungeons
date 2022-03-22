#pragma once

namespace Internal {

	struct Vector3D;
	struct Matrix3;
	struct Matrix3D;
	struct Plane;

	inline float flabs(float f) { return (f >= 0.0f ? f : -f); }
	inline bool fuzzyIsNull(float f)
	{
		return std::fabs(f) < 1e-8f;
	}

	Vector3D operator*(float scalar, const Vector3D& v);
	/** Dot product. */
	float    operator*(const Vector3D& v1, const Vector3D& v2);
	Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
	Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
	Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2);
	Matrix3D operator*(const Matrix3D& m1, const Matrix3D& m2);
	Matrix3D operator*(float scalar, const Matrix3D& m);

	struct Vector3D
	{
		float x, y, z;
		static const Vector3D Zero;

		Vector3D() {}
		Vector3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
		Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}

		Vector3D& operator+=(const Vector3D& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vector3D& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
		Vector3D& operator/=(float s) { return *this *= (1.0f / s); }

		const float* constData() const { return &x; }

		Vector3D operator-       () const { return Vector3D(-x, -y, -z); }
		float    SquareMagnitude() const { return x * x + y * y + z * z; }
		float    Magnitude() const { return (float)sqrt(SquareMagnitude()); }
		Vector3D Normalized() const { return (1.0f / Magnitude()) * (*this); }
		float    operator[] (int i) const { return ((float*)&x)[i]; }
		float& operator[] (int i) { return ((float*)&x)[i]; }

		// Casting tools
		static Vector3D* asPointer(float* floatPtr);
		static const Vector3D* asConstPointer(const float* floatPtr);
		static Vector3D& asRef(float* floatPtr);
		static const Vector3D& asConstRef(const float* floatPtr);

		// Float array tools
		static void fillFloatVec(float vec[3], float value);
		static void setFloatVec(float vec[3], float x, float y, float z);
	};

	/** 3x3 matrix */
	struct Matrix3
	{
		union {
			struct {
				float s11, s12, s13,
					s21, s22, s23,
					s31, s32, s33;
			} sclr;
			float m[3][3];
		};
		static const Matrix3 Identity;

		Vector3D& baseRow(int i) { return *((Vector3D*)m[i]); }
		float  operator() (int i, int j) const { return m[i][j]; }
		float& operator() (int i, int j) { return m[i][j]; }
	};

	/** 4x4 matrix, used for transformations. */
	struct Matrix3D
	{
		union {
			struct {
				float s11, s12, s13, s14,
					s21, s22, s23, s24,
					s31, s32, s33, s34,
					s41, s42, s43, s44;
			} sclr;
			float m[4][4];
		};
		static const Matrix3D Identity;

		Matrix3D() {}

		Matrix3D(float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44)
		{
			sclr.s11 = f11; sclr.s12 = f12; sclr.s13 = f13; sclr.s14 = f14;
			sclr.s21 = f21; sclr.s22 = f22; sclr.s23 = f23; sclr.s24 = f24;
			sclr.s31 = f31; sclr.s32 = f32; sclr.s33 = f33; sclr.s34 = f34;
			sclr.s41 = f41; sclr.s42 = f42; sclr.s43 = f43; sclr.s44 = f44;
		}

		Matrix3D& operator*= (const Matrix3D& m)
		{
			return *this = *this * m;
		}

		friend Matrix3D PitchMatrix3D(const float theta);
		friend Matrix3D YawMatrix3D(const float theta);
		friend Matrix3D RollMatrix3D(const float theta);
		void rotate(const Vector3D& v);

		Matrix3D Inverse() const;
		Matrix3D Adjoint() const;
		float Determinant() const;

		float  operator() (int i, int j) const { return m[i][j]; }
		float& operator() (int i, int j) { return m[i][j]; }
	};

	/** 3D Plane.  Used in conjunction with triangles. */
	struct Plane
	{
		Vector3D normal;
		float    d;

		Plane(const Vector3D& a, const Vector3D& b, const Vector3D& c)
		{
			normal = CrossProduct(b - a, c - a).Normalized();
			d = -normal * a;
		}

		float Classify(const Vector3D& v) const
		{
			return v * this->normal + this->d;
		}
	};

	inline Vector3D operator* (float scalar, const Vector3D& v)
	{
		return Vector3D(scalar * v.x, scalar * v.y, scalar * v.z);
	}

	inline Vector3D operator+ (const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	inline Vector3D operator- (const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	inline float operator* (const Vector3D& v1, const Vector3D& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2)
	{
		return Vector3D(v1.y * v2.z - v2.y * v1.z,
			v1.z * v2.x - v2.z * v1.x,
			v1.x * v2.y - v2.x * v1.y);
	}

	inline Vector3D Transform(const Vector3D& v, const Matrix3D& m)
	{
		return Vector3D(v.x * m.sclr.s11 + v.y * m.sclr.s21 + v.z * m.sclr.s31 + m.sclr.s41,
			v.x * m.sclr.s12 + v.y * m.sclr.s22 + v.z * m.sclr.s32 + m.sclr.s42,
			v.x * m.sclr.s13 + v.y * m.sclr.s23 + v.z * m.sclr.s33 + m.sclr.s43);
	}

	inline Vector3D rotateVector(const Vector3D& v, const Matrix3D& m)
	{
		return Vector3D(v.x * m.sclr.s11 + v.y * m.sclr.s21 + v.z * m.sclr.s31,
			v.x * m.sclr.s12 + v.y * m.sclr.s22 + v.z * m.sclr.s32,
			v.x * m.sclr.s13 + v.y * m.sclr.s23 + v.z * m.sclr.s33);
	}

	inline Matrix3D operator*(float scalar, const Matrix3D& m)
	{
		return Matrix3D(scalar * m(0, 0), scalar * m(0, 1), scalar * m(0, 2), scalar * m(0, 3),
			scalar * m(1, 0), scalar * m(1, 1), scalar * m(1, 2), scalar * m(1, 3),
			scalar * m(2, 0), scalar * m(2, 1), scalar * m(2, 2), scalar * m(2, 3),
			scalar * m(3, 0), scalar * m(3, 1), scalar * m(3, 2), scalar * m(3, 3));
	}

	inline Matrix3D operator*(const Matrix3D& m1, const Matrix3D& m2)
	{
		return Matrix3D(
			m1.sclr.s11 * m2.sclr.s11 + m1.sclr.s12 * m2.sclr.s21 + m1.sclr.s13 * m2.sclr.s31 + m1.sclr.s14 * m2.sclr.s41,
			m1.sclr.s11 * m2.sclr.s12 + m1.sclr.s12 * m2.sclr.s22 + m1.sclr.s13 * m2.sclr.s32 + m1.sclr.s14 * m2.sclr.s42,
			m1.sclr.s11 * m2.sclr.s13 + m1.sclr.s12 * m2.sclr.s23 + m1.sclr.s13 * m2.sclr.s33 + m1.sclr.s14 * m2.sclr.s43,
			m1.sclr.s11 * m2.sclr.s14 + m1.sclr.s12 * m2.sclr.s24 + m1.sclr.s13 * m2.sclr.s34 + m1.sclr.s14 * m2.sclr.s44,
			m1.sclr.s21 * m2.sclr.s11 + m1.sclr.s22 * m2.sclr.s21 + m1.sclr.s23 * m2.sclr.s31 + m1.sclr.s24 * m2.sclr.s41,
			m1.sclr.s21 * m2.sclr.s12 + m1.sclr.s22 * m2.sclr.s22 + m1.sclr.s23 * m2.sclr.s32 + m1.sclr.s24 * m2.sclr.s42,
			m1.sclr.s21 * m2.sclr.s13 + m1.sclr.s22 * m2.sclr.s23 + m1.sclr.s23 * m2.sclr.s33 + m1.sclr.s24 * m2.sclr.s43,
			m1.sclr.s21 * m2.sclr.s14 + m1.sclr.s22 * m2.sclr.s24 + m1.sclr.s23 * m2.sclr.s34 + m1.sclr.s24 * m2.sclr.s44,
			m1.sclr.s31 * m2.sclr.s11 + m1.sclr.s32 * m2.sclr.s21 + m1.sclr.s33 * m2.sclr.s31 + m1.sclr.s34 * m2.sclr.s41,
			m1.sclr.s31 * m2.sclr.s12 + m1.sclr.s32 * m2.sclr.s22 + m1.sclr.s33 * m2.sclr.s32 + m1.sclr.s34 * m2.sclr.s42,
			m1.sclr.s31 * m2.sclr.s13 + m1.sclr.s32 * m2.sclr.s23 + m1.sclr.s33 * m2.sclr.s33 + m1.sclr.s34 * m2.sclr.s43,
			m1.sclr.s31 * m2.sclr.s14 + m1.sclr.s32 * m2.sclr.s24 + m1.sclr.s33 * m2.sclr.s34 + m1.sclr.s34 * m2.sclr.s44,
			m1.sclr.s41 * m2.sclr.s11 + m1.sclr.s42 * m2.sclr.s21 + m1.sclr.s43 * m2.sclr.s31 + m1.sclr.s44 * m2.sclr.s41,
			m1.sclr.s41 * m2.sclr.s12 + m1.sclr.s42 * m2.sclr.s22 + m1.sclr.s43 * m2.sclr.s32 + m1.sclr.s44 * m2.sclr.s42,
			m1.sclr.s41 * m2.sclr.s13 + m1.sclr.s42 * m2.sclr.s23 + m1.sclr.s43 * m2.sclr.s33 + m1.sclr.s44 * m2.sclr.s43,
			m1.sclr.s41 * m2.sclr.s14 + m1.sclr.s42 * m2.sclr.s24 + m1.sclr.s43 * m2.sclr.s34 + m1.sclr.s44 * m2.sclr.s44);
	}

	inline Matrix3D PitchMatrix3D(const float c, const float s)
	{
		return Matrix3D(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, c, -s, 0.0f,
			0.0f, s, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}


	inline Matrix3D PitchMatrix3D(const float theta)
	{
		return PitchMatrix3D((float)cos(theta), (float)sin(theta));
	}


	inline Matrix3D YawMatrix3D(const float c, const float s)
	{
		return Matrix3D(c, 0.0f, s, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-s, 0.0f, c, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}


	inline Matrix3D YawMatrix3D(const float theta)
	{
		return YawMatrix3D((float)cos(theta), (float)sin(theta));
	}


	inline Matrix3D RollMatrix3D(const float c, const float s)
	{
		return Matrix3D(c, -s, 0.0f, 0.0f,
			s, c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}


	inline Matrix3D RollMatrix3D(const float theta)
	{
		return RollMatrix3D((float)cos(theta), (float)sin(theta));
	}


	inline void Matrix3D::rotate(const Vector3D& v)
	{
		if (v.x != 0.0f) *this = PitchMatrix3D(v.x) * (*this);
		if (v.y != 0.0f) *this = YawMatrix3D(v.y) * (*this);
		if (v.z != 0.0f) *this = RollMatrix3D(v.z) * (*this);
	}

	inline Matrix3D TranslateMatrix3D(const Vector3D& v)
	{
		return Matrix3D(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			v.x, v.y, v.z, 1.0f);
	}


	inline Matrix3D ScaleMatrix3D(const Vector3D& v)
	{
		return Matrix3D(
			v.x, 0.0f, 0.0f, 0.0f,
			0.0f, v.y, 0.0f, 0.0f,
			0.0f, 0.0f, v.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}


	inline Matrix3D ScaleMatrix3D(const float s)
	{
		return ScaleMatrix3D(Vector3D(s, s, s));
	}

} // namespace Internal