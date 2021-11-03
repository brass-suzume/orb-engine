#pragma once

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