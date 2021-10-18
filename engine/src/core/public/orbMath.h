#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
using json = nlohmann::json;

namespace core
{
	using Rect = DirectX::SimpleMath::Rectangle;
	constexpr float NUM_PI = 3.14159265358979323846f;

	float GetRoll( const Matrix& m );
	bool IsZero( float val, float eps = FLT_EPSILON );

	constexpr float RadToDeg( float rad )
	{
		return ( rad * 180.0f ) / NUM_PI;
	}

	constexpr float DegToRad( float deg )
	{
		return ( deg * NUM_PI ) / 180.0f;
	}
}

namespace DirectX
{
	namespace SimpleMath
	{
		void to_json( json& j, const Vector2& vec );
		void from_json( const json& j, Vector2& vec );

		void to_json( json& j, const Color& color );
		void from_json( const json& j, Color& color );
	}
}