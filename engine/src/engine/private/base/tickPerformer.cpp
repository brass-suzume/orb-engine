#include "tickPerformer.h"

namespace core
{
	TickPerformer::TickPerformer()
	{
		m_tickGroups.resize( static_cast< size_t >( TickGroup::_COUNT ), Group{} );
	}

	void TickPerformer::RegisterForTick( TickGroup group, TickFunctor&& functor )
	{
		m_tickGroups[ static_cast< size_t >( group ) ].m_tickFunctors.push_back( std::move( functor ) );
	}

	void TickPerformer::Tick( const TickInfo& tickInfo )
	{
		for( const Group& group : m_tickGroups )
		{
			for( const TickFunctor& tickFunc : group.m_tickFunctors )
			{
				tickFunc( tickInfo );
			}
		}
	}
}