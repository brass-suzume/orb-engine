#include "gameMode.h"

#include "entityRef.h"


ORB_REGISTER_TYPE_IN_NS( core, GameMode );

namespace core
{
	bool GameMode::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_playerEntityResPath );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_spawnPlayer );
		return Super::Deserialize( data );
	}

	void GameMode::Init( GameInstance* gameInstance )
	{
		m_gameInstance = gameInstance;
		OnInit();
	}

	void GameMode::Uninit()
	{
		OnUninit();
		m_gameInstance = nullptr;
	}

	void GameMode::Tick( float dt )
	{
	}
	
	void GameMode::OnInit()
	{
	}

	void GameMode::OnUninit()
	{
	}
}