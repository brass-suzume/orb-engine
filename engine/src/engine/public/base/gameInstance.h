#pragma once

#include "gameSystem.h"

namespace core
{
	class IGameSystem;
	class TickInfo;
	class TickPerformer;
	class GameMode;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct GameInstanceInitCtx
	{
		std::weak_ptr< IRenderer > m_renderer;
		const char* m_playerObjectTypeName = nullptr;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class GameInstance final : private NoCopy
	{

	public:
		GameInstance();
		~GameInstance();

		template< class TSystem >
		void AddSystem()
		{
			ORB_ASSERT( m_systems.count( TSystem::GetStaticID() ) == 0, "System already added" );
			m_systems[ TSystem::GetStaticID() ] = GameSystemRegistrar::GetInstance().CreateSystem< TSystem >();
		}

		void InitSystems( const GameInstanceInitCtx& initCtx );
		void UninitSystems();

		void Tick( const TickInfo& tickInfo );
		
		void RequestLoadLevel( const char* levelResPath );
		void RequestQuit();
		void RequestRestart();

		bool HasQuitRequest() const { return m_quit; }

		IGameSystem& GetSystem( SystemID systemId ) const;
		
		template< class TSystem >
		TSystem& GetSystem() const
		{
			return dynamic_cast< TSystem& >( GetSystem( TGameSystem< TSystem >::GetStaticID() ) );
		}

		std::shared_ptr< IRenderer > GetRenderer() const { return m_renderer.lock(); }
		std::shared_ptr< Level > GetLevel() const { return m_level; }
		std::shared_ptr< GameMode > GetGameMode() const;

	private:
		void ProcessLoadLevelRequest();
		void LoadLevel( const std::string& levelResPath );
		void UnloadCurrentLevel();

	private:
		std::unordered_map< SystemID, std::shared_ptr< IGameSystem > > m_systems;
		std::unique_ptr< TickPerformer > m_tickPerformer;
		std::weak_ptr< IRenderer > m_renderer;

		bool m_loadNewLevel = false;
		std::string m_levelResPath;
		std::shared_ptr< Level > m_level;

		bool m_quit = false;
		bool m_restart = false;
	};
}