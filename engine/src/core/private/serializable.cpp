#include "serializable.h"


ORB_REGISTER_ABSTRACT_TYPE( Serializable );

namespace core
{
	bool Serializable::Deserialize( const json& data )
	{
		return true;
	}
}