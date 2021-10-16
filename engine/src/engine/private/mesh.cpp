#include "mesh.h"


ORB_REGISTER_TYPE( Mesh );

namespace core
{
	bool Mesh::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_name );
		ORB_DESERIALIZE_PROPERTY( m_vertices );
		ORB_DESERIALIZE_PROPERTY( m_indices );
		ORB_DESERIALIZE_PROPERTY( m_boundingSphere );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_colorPerTriangle );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_color );
		return Super::Deserialize( data );
	}
}