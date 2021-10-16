#pragma once

namespace core
{
	class EngineRuntimeStage;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct EngineInitData
	{

	public:
		EngineInitData( const char* windowName, std::unique_ptr< EngineRuntimeStage >&& engineStage, const char* pathToStartingLevel );
		~EngineInitData();

	public:
		const char* m_windowName = nullptr;
		std::unique_ptr< EngineRuntimeStage > m_runtimeStage;
		const char* m_startingLevelResPath = nullptr;
	};
}