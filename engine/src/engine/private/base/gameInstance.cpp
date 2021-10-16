#include "gameInstance.h"

#include "tickPerformer.h"
#include "level.h"
#include "rendererInterface.h"


namespace core
{
	GameInstance::GameInstance() = default;

	GameInstance::~GameInstance()
	{
		ORB_ASSERT( m_systems.empty(), "Game systems are still present" );
	}

	void GameInstance::InitSystems( const GameInstanceInitCtx& initCtx )
	{
		m_tickPerformer = std::make_unique< TickPerformer >();
		m_renderer = initCtx.m_renderer;

		const GameSystemInitContext systemInitCtx{ *this, *m_tickPerformer, initCtx.m_playerObjectTypeName };
		for( const auto& it : m_systems )
		{
			it.second->Init( systemInitCtx );
		}
	}

	void GameInstance::UninitSystems()
	{
		if( m_level )
		{
			UnloadCurrentLevel();
		}

		for( const auto& it : m_systems )
		{
			it.second->Uninit();
		}
		m_systems.clear();

		m_renderer.reset();
		m_tickPerformer.reset();
	}

	void GameInstance::Tick( const TickInfo& tickInfo )
	{
		if( m_quit )
		{
			return;
		}

		ProcessLoadLevelRequest();

		m_level->Tick( tickInfo.GetDeltaTime() );
		m_tickPerformer->Tick( tickInfo );
	}

	void GameInstance::RequestLoadLevel( const char* levelResPath )
	{
		m_loadNewLevel = true;
		m_levelResPath = levelResPath;
	}

	void GameInstance::RequestQuit()
	{
		m_quit = true;
	}

	void GameInstance::RequestRestart()
	{
		m_restart = true;
	}

	IGameSystem& GameInstance::GetSystem( SystemID systemId ) const
	{
		auto& sys = m_systems.at( systemId );
		ORB_ASSERT( sys, "Failed to find system" );
		return *sys;
	}

	std::shared_ptr< GameMode > GameInstance::GetGameMode() const
	{
		return m_level->GetGameMode();
	}

	void GameInstance::ProcessLoadLevelRequest()
	{
		if( m_loadNewLevel || m_restart )
		{
			if( m_level )
			{
				UnloadCurrentLevel();
			}

			LoadLevel( m_levelResPath );

			m_loadNewLevel = false;
			m_restart = false;
		}
	}

	void GameInstance::LoadLevel( const std::string& levelResPath )
	{
		m_level = DataCenter::GetInstance().CreateSerializableFromResource< Level >( levelResPath );
		m_level->Init( this );

		for( const auto& it : m_systems )
		{
			it.second->OnLevelLoad( m_level );
		}

		m_renderer.lock()->SetSkybox( DataCenter::GetInstance().GetFullResourcePath( m_level->GetSkybox() ) );
	}

	void GameInstance::UnloadCurrentLevel()
	{
		for( const auto& it : m_systems )
		{
			it.second->OnLevelUnload( m_level );
		}

		m_level->Uninit();
		m_level.reset();
	}
}