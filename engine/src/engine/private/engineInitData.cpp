#include "engineInitData.h"

#include "engineRuntimeStage.h"

namespace core
{
	EngineInitData::EngineInitData( const char* windowName, std::unique_ptr< EngineRuntimeStage >&& engineStage, const char* startingLevelResPath )
		: m_windowName{ windowName }
		, m_runtimeStage{ std::move( engineStage ) }
		, m_startingLevelResPath{ startingLevelResPath }
	{
	}

	EngineInitData::~EngineInitData()
	{
	}
}