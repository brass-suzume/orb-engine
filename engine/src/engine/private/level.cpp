#include "level.h"

#include "entityRef.h"
#include "gameMode.h"


ORB_REGISTER_TYPE_IN_NS( core, Level );

namespace core
{
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	void to_json( json& j, const LevelSpawnPoint& sp )
	{
	}

	void from_json( const json& j, LevelSpawnPoint& sp )
	{
		sp.m_position = Vector2{ j[ 0 ], j[ 1 ] };
		sp.m_rotation = DegToRad( j[ 2 ] );
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	bool Level::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_levelName );
		ORB_DESERIALIZE_OBJECT_PROPERTY( m_gameMode );
		ORB_DESERIALIZE_OBJECT_ARRAY_PROPERTY( m_levelEntities );
		ORB_DESERIALIZE_PROPERTY( m_skybox );

		return Super::Deserialize( data );
	}

	void Level::Init( GameInstance* gameInstance )
	{
		OnInit( gameInstance );
		m_gameMode->Init( gameInstance );
	}

	void Level::Uninit()
	{
		m_gameMode->Uninit();
		OnUninit();
	}

	void Level::Tick( float dt )
	{
		m_gameMode->Tick( dt );
	}

	void Level::OnInit( GameInstance* gameInstance )
	{
	}

	void Level::OnUninit()
	{
	}
}