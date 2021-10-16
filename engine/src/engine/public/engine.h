#pragma once

#include "engineStage.h"

namespace core
{
	struct EngineInitData;
	class EngineRuntimeStage;

	using EngineStagePtr = std::unique_ptr< EngineStage >;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Engine final
	{

	public:
		Engine();
		~Engine();

		void Start( EngineInitData&& initData );

	private:
		bool Initialize( EngineInitData&& initData );
		void Uninitialize();

		void RunEngine();

		template< class TStage, class TInitData >
		TStage* AddStage( std::unique_ptr< TStage >&& stage, const TInitData& initData );

		template< class TStage, class TInitData >
		TStage* AddStage( const TInitData& initData );

	private:
		std::vector< EngineStagePtr > m_engineStages;
		EngineRuntimeStage* m_runtimeStage;
	};
}