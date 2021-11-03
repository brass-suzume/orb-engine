#include "controllerSystem.h"

#include "player.h"
#include "tickRegister.h"
#include "level.h"
#include "gameInstance.h"
#include "entitySystemInterface.h"
#include "gameMode.h"
#include "entity.h"


ORB_REGISTER_SYSTEM( ControllerSystem );

namespace core
{
	ControllerSystem::ControllerSystem() = default;

	ControllerSystem::~ControllerSystem() = default;

	void ControllerSystem::CreatePlayerEntity( const std::string& resPath, const LevelSpawnPoint& spawnPoint )
	{
		auto creationCb = [ weakSelf = MakeMeWeak< ControllerSystem >( shared_from_this() ) ]( const std::shared_ptr< Entity >& ent )
		{
			if( auto self = weakSelf.lock() )
			{
				self->SetPlayerEntity( ent );
			}
		};

		auto& entitySys = GetGameInstance()->GetSystem< IEntitySystem >();
		entitySys.CreateEntity( resPath, spawnPoint.m_position, spawnPoint.m_rotation, std::move( creationCb ), nullptr );
	}

	void ControllerSystem::DestroyPlayerEntity()
	{
		Entry& playerEntry = m_controllers[ 0 ];
		ReleaseControl( playerEntry );
	}

	std::shared_ptr< Entity > ControllerSystem::GetPlayerEntity() const
	{
		return m_controllers[ 0 ].m_entity;
	}

	std::shared_ptr< Controller > ControllerSystem::GetPlayerController() const
	{
		return m_controllers[ 0 ].m_controller;
	}

	std::shared_ptr< Controller > ControllerSystem::RequestNewController( const std::string& controllerTypeName, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint )
	{
		auto newController = DataCenter::GetInstance().CreateSerializableFromTypeName< Controller >( controllerTypeName );
		AddController( newController );
		CreateControllerEntity( newController, botEntityResPath, spawnPoint );
		return newController;
	}

	ControllerEntityChangedCbReg::ID ControllerSystem::RegisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::TCallbackFunctor&& callback )
	{
		return m_cbRegister.RegisterCallback( std::move( callback ) );
	}

	void ControllerSystem::UnregisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::ID& callbackID )
	{
		m_cbRegister.UnregisterCallback( callbackID );
	}

	void ControllerSystem::GetControllerEntities( std::vector< std::shared_ptr< Entity > >& entities ) const
	{
		entities.reserve( m_controllers.size() );
		for( const Entry& entry : m_controllers )
		{
			if( entry.m_entity )
			{
				entities.push_back( entry.m_entity );
			}
		}
	}

	void ControllerSystem::ClearNonplayers()
	{
		for( size_t i = 1; i < m_controllers.size(); ++i )
		{
			ReleaseControl( m_controllers[ i ] );
		}
		m_controllers.erase( m_controllers.begin() + 1, m_controllers.end() );
	}

	void ControllerSystem::CreateControllerEntity( const std::shared_ptr< Controller >& controller, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint )
	{
		auto it = std::find_if( m_controllers.begin(), m_controllers.end(), [controller]( const Entry& entry )
		{
			return entry.m_controller == controller;
		} );

		ORB_ASSERT( !it->m_entity );

		std::weak_ptr< Controller > weakController = controller;
		auto creationCb = [this, weakController]( const std::shared_ptr< Entity >& ent )
		{
			if( auto controller = weakController.lock() )
			{
				SetControllerEntity( controller, ent );
			}
		};

		auto& entitySys = GetGameInstance()->GetSystem< IEntitySystem >();
		entitySys.CreateEntity( botEntityResPath, spawnPoint.m_position, spawnPoint.m_rotation, std::move( creationCb ), nullptr );
	}

	void ControllerSystem::DestroyControllerEntity( const std::shared_ptr< Controller >& controller )
	{
		auto it = std::find_if( m_controllers.begin(), m_controllers.end(), [controller]( const Entry& entry )
		{
			return entry.m_controller == controller;
		} );

		ORB_ASSERT( it->m_entity );

		ReleaseControl( *it );
	}

	std::shared_ptr< Controller > ControllerSystem::GetControllerForEntity( const std::shared_ptr< Entity >& entity ) const
	{
		ORB_ASSERT( entity );
		auto it = std::find_if( m_controllers.begin(), m_controllers.end(), [&entity]( const Entry& entry )
		{
			return entry.m_entity == entity;
		} );

		if( it != m_controllers.end() )
		{
			return it->m_controller;
		}

		return nullptr;
	}

	void ControllerSystem::OnInit( const GameSystemInitContext& initCtx )
	{
		AddController( DataCenter::GetInstance().CreateSerializableFromTypeName< Player >( initCtx.m_playerObjectTypename ) );

		initCtx.m_tickRegister.RegisterForTick( TickGroup::Game, [this]( const TickInfo& tickInfo )
		{
			UpdateControllers( tickInfo.GetDeltaTime() );
		} );
	}

	void ControllerSystem::OnUninit()
	{
		for( const Entry& entry : m_controllers )
		{
			ORB_ASSERT( !entry.m_entity );
			entry.m_controller->Uninit();
		}
		m_controllers.clear();
	}

	void ControllerSystem::AddController( const std::shared_ptr< Controller >& c )
	{
		m_controllers.push_back( { c, nullptr } );
		c->Init( *GetGameInstance() );
	}

	void ControllerSystem::SetPlayerEntity( const std::shared_ptr< Entity >& entity )
	{
		Entry& playerEntry = m_controllers[ 0 ];
		playerEntry.m_entity = entity;
		playerEntry.m_controller->SetControlledEntity( entity );

		NotifyListeners( playerEntry.m_controller );
	}

	void ControllerSystem::SetControllerEntity( const std::shared_ptr< Controller >& c, const std::shared_ptr< Entity >& entity )
	{
		auto findIt = std::find_if( m_controllers.begin(), m_controllers.end(), [c]( const Entry& entry )
		{
			return entry.m_controller == c;
		} );

		findIt->m_entity = entity;
		findIt->m_controller->SetControlledEntity( entity );

		NotifyListeners( findIt->m_controller );
	}

	void ControllerSystem::ReleaseControl( Entry& entry )
	{
		entry.m_controller->SetControlledEntity( nullptr );

		std::shared_ptr< Entity > entity = std::move( entry.m_entity );
		if( entity )
		{
			auto& entitySys = GetGameInstance()->GetSystem< IEntitySystem >();
			entitySys.DestroyEntity( entity->GetEntityID() );
		}

		NotifyListeners( entry.m_controller );
	}

	void ControllerSystem::UpdateControllers( float dt )
	{
		for( const Entry& entry : m_controllers )
		{
			entry.m_controller->Update( dt );
		}
	}

	void ControllerSystem::NotifyListeners( const std::shared_ptr< Controller >& affectedController )
	{
		m_cbRegister.Notify( affectedController );
	}
}