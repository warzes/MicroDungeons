#include "stdafx.h"
#include "SphereCollisionTest.h"
#include "TinyMath3D.h"

SphereCollisionTest::SphereCollisionTest()
	: m_sphereRadius(0)
{
	Internal::Vector3D::fillFloatVec(m_sphereCenter, 0);
}

const float* SphereCollisionTest::sphereCenter() const
{
	return m_sphereCenter;
}

void SphereCollisionTest::setSphereCenter(float x, float y, float z)
{
	Internal::Vector3D::setFloatVec(m_sphereCenter, x, y, z);
}

float SphereCollisionTest::sphereRadius() const
{
	return m_sphereRadius;
}

void SphereCollisionTest::setSphereRadius(float radius)
{
	m_sphereRadius = radius;
}