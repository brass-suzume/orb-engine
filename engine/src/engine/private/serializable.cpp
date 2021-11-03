#include "serializable.h"


ORB_REGISTER_ABSTRACT_TYPE_IN_NS( core, Serializable );

namespace core
{
	bool Serializable::Deserialize( const json& data )
	{
		return true;
	}
}