#pragma once

#include "engine/public/engineRuntimeStage.h"

namespace core
{
	class GameInstance;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class GameRuntimeStage : public EngineRuntimeStage
	{

	public:
		GameRuntimeStage();
		virtual ~GameRuntimeStage();

	protected:
		virtual bool OnInit( const RuntimeStageInitData& initData ) override;
		virtual void OnUninit() override;
		virtual void OnFrameTick( const TickInfo& tickInfo ) override;

		GameInstance& GetGameInstance();
		const GameInstance& GetGameInstance() const;

	private:
		virtual void CreateSystems();
		virtual const char* GetPlayerObjectTypeName() const;

	private:
		std::unique_ptr< GameInstance > m_gameInstance;
	};
}