#include "entityRef.h"


ORB_REGISTER_TYPE( EntityRef );

namespace core
{
	EntityRef::EntityRef() = default;

	EntityRef::EntityRef( const std::string& resPath, const Vector2& position, float rotation )
		: Super{ resPath }
		, m_position{ position }
		, m_rotation{ rotation }
	{
	}

	bool EntityRef::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_position );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_rotation );
		return Super::Deserialize( data );
	}
}