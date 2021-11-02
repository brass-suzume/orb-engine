#include "serializationHelpers.h"

namespace core
{
	std::string ExtractRealPropertyName( const char* propertyName )
	{
		if( strncmp( propertyName, "m_", 2 ) == 0 )
		{
			return std::string{ propertyName + 2, strlen( propertyName ) - 2 };
		}
		else
		{
			return std::string{ propertyName, strlen( propertyName ) };
		}
	}
}