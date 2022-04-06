#pragma once

constexpr float EpsilonF = std::numeric_limits<float>::epsilon();

// Clamp float value
inline float Clamp(float value, float min, float max)
{
	float result = (value < min) ? min : value;
	if (result > max) result = max;
	return result;
}

// Calculate linear interpolation between two floats
inline float Lerp(float start, float end, float amount)
{
	return start + amount * (end - start);
}

// Normalize input value within input range
inline float Normalize(float value, float start, float end)
{
	return (value - start) / (end - start);
}