#pragma once

#include "tickRegister.h"

namespace core
{
	class TickPerformer : public ITickRegister
	{

	private:
		struct Group
		{
			std::vector< TickFunctor > m_tickFunctors;
		};

	public:
		TickPerformer();

		virtual void RegisterForTick( TickGroup group, TickFunctor&& functor ) override;

		void Tick( const TickInfo& tickInfo );

	private:
		std::vector< Group > m_tickGroups;
	};
}