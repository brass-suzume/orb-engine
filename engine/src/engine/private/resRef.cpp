#include "resRef.h"


ORB_REGISTER_TYPE_IN_NS( core, ResRef );

namespace core
{
	ResRef::ResRef() = default;

	ResRef::ResRef( const std::string& resPath )
		: m_resPath{ resPath }
	{}

	bool ResRef::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_resPath );
		return Super::Deserialize( data );
	}
}