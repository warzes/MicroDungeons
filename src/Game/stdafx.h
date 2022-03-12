#pragma once

#if defined(_MSC_VER)
#	pragma warning(push, 0)
//#	pragma warning(disable : 4365)
#endif

#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <rlFPCamera.h>
#include <RLFrustum.h>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <set>
#include <queue>
#include <array>
#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#include "Globals.h"