#pragma once

#if defined(_MSC_VER)
#	pragma warning(disable : 4514)
#	pragma warning(disable : 4626)
#	pragma warning(disable : 4820)
#	pragma warning(disable : 5027)
#	pragma warning(disable : 5045)
#	pragma warning(push, 0)
//#	pragma warning(disable : 4365)
#endif

#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <RaylibOperators.h>
#include <rlFPCamera.h>
#include <RLFrustum.h>

#include <glm/glm.hpp>

#include <iostream>
#include <set>
#include <queue>
#include <array>
#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#include "Globals.h"


inline Vector3 Mix(const Vector3& x, const Vector3& y, float a)
{
	return (x * (1.0f - a) + y * a);
}