#pragma once

namespace math
{
	constexpr float NUM_PI = 3.14159265358979323846f;
	constexpr float NUM_PI_DIV_2 = NUM_PI / 2.0f;

	float GetRoll( const Matrix& m );
	bool IsZero( float val, float eps = FLT_EPSILON );

	constexpr float RadToDeg( float rad );
	constexpr float DegToRad( float deg );
}

#include "mathUtils.hpp"