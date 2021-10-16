#include "gameRuntimeStage.h"

#include "gameInstance.h"
#include "player.h"


namespace core
{
	GameRuntimeStage::GameRuntimeStage() = default;

	GameRuntimeStage::~GameRuntimeStage()
	{
		ORB_ASSERT( !m_gameInstance, "Game instance still exists" );
	}

	bool GameRuntimeStage::OnInit( const RuntimeStageInitData& initData )
	{
		m_gameInstance = std::make_unique< GameInstance >();

		CreateSystems();

		const GameInstanceInitCtx initCtx{ initData.m_renderer, GetPlayerObjectTypeName() };

		m_gameInstance->InitSystems( initCtx );
		m_gameInstance->RequestLoadLevel( initData.m_startingLevelResPath );

		return true;
	}

	void GameRuntimeStage::OnUninit()
	{
		m_gameInstance->UninitSystems();
		m_gameInstance.reset();
	}

	void GameRuntimeStage::OnFrameTick( const TickInfo& tickInfo )
	{
		if( !m_gameInstance->HasQuitRequest() )
		{
			m_gameInstance->Tick( tickInfo );
		}
		else
		{
			Shutdown();
		}
	}

	GameInstance& GameRuntimeStage::GetGameInstance()
	{
		return *m_gameInstance;
	}

	const GameInstance& GameRuntimeStage::GetGameInstance() const
	{
		return *m_gameInstance;
	}

	void GameRuntimeStage::CreateSystems()
	{
	}

	const char* GameRuntimeStage::GetPlayerObjectTypeName() const
	{
		return Player::GetTypeName();
	}
}