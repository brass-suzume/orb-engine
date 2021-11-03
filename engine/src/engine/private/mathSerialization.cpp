#include "mathSerialization.h"


namespace DirectX
{
	namespace SimpleMath
	{
		void to_json( json& j, const Vector2& vec )
		{
			j = json{ { vec.x, vec.y } };
		}

		void from_json( const json& j, Vector2& vec )
		{
			vec.x = j[ 0 ];
			vec.y = j[ 1 ];
		}

		void to_json( json& j, const Color& color )
		{
			j = json{ color.RGBA().v };
		}

		void from_json( const json& j, Color& color )
		{
			DirectX::PackedVector::XMUBYTEN4 byte4;
			j.get_to( byte4.v );
			DirectX::PackedVector::XMUBYTEN4 colorPack;
			colorPack.x = byte4.w;
			colorPack.y = byte4.z;
			colorPack.z = byte4.y;
			colorPack.w = byte4.x;
			color = Color{ colorPack };
		}
	}
}