#include "entitySystem.h"

#include "entity.h"
#include "entityRef.h"
#include "tickRegister.h"
#include "level.h"
#include "positionComponent.h"


ORB_REGISTER_SYSTEM( EntitySystem );

namespace core
{
	EntityID EntitySystem::CreateEntity( const std::string& resPath, const Vector2& position, float rotation, EntityCreatedCallback&& callback, const std::shared_ptr< Entity >& owner )
	{
		const EntityID newId = ++m_entityIdGen;
		m_entitiesToCreate.push_back( { newId, resPath, position, rotation, std::move( callback ), owner } );
		return newId;
	}

	void EntitySystem::DestroyEntity( EntityID entityId )
	{
		auto findIt = m_entities.find( entityId );
		if( findIt != m_entities.end() )
		{
			findIt->second->Uninit();
			m_entities.erase( findIt );
		}
		else
		{
			auto removeIt = std::remove_if( m_entitiesToCreate.begin(), m_entitiesToCreate.end(), [entityId]( const CreateEntry& entry )
			{
				return entityId == entry.m_entityId;
			} );

			ORB_ASSERT( removeIt != m_entitiesToCreate.end() );
			m_entitiesToCreate.erase( removeIt, m_entitiesToCreate.end() );
		}
	}

	void EntitySystem::RegisterEntityRef( const std::shared_ptr< EntityRef >& ref, EntityRefRegisteredCallback&& callback )
	{
		m_refsToProcess.push_back( { ref, std::move( callback ) } );
	}

	void EntitySystem::UnregisterEntityRef( const std::shared_ptr< EntityRef >& ref )
	{
		if( !RemoveEntityRef( ref ) )
		{
			auto it = std::find_if( m_refsToProcess.begin(), m_refsToProcess.end(), [&ref]( const RegisterEntry& entry )
			{
				return entry.m_ref.lock() == ref;
			} );

			ORB_ASSERT( it != m_refsToProcess.end(), "Invalid state" );
			m_refsToProcess.erase( it );
		}
	}

	void EntitySystem::SetEntityTransform( EntityID entityId, const Vector2& position, float rotation )
	{
		UpdateTransformEntry& entry = m_transformsToUpdate[ entityId ];
		entry.m_entityUpdate = true;
		entry.m_entityData.m_position = position;
		entry.m_entityData.m_rotation = rotation;
	}

	void EntitySystem::SetComponentTransform( const std::shared_ptr< PositionComponent >& component, const Vector2& position, float rotation )
	{
		UpdateTransformEntry& entry = m_transformsToUpdate[ component->GetEntityOwner()->GetEntityID() ];
		auto it = std::find_if( entry.m_components.begin(), entry.m_components.end(), [ &component ]( const UpdateTransformEntry::ComponentData& d )
		{
			return d.m_component.lock() == component;
		} );

		if( it == entry.m_components.end() )
		{
			entry.m_components.emplace_back();
			UpdateTransformEntry::ComponentData& componentEntry = entry.m_components.back();
			componentEntry.m_component = component;
			componentEntry.m_data.m_position = position;
			componentEntry.m_data.m_rotation = rotation;
		}
		else
		{
			it->m_data.m_position = position;
			it->m_data.m_rotation = rotation;
		}
	}

	void EntitySystem::OnLevelLoad( const std::shared_ptr< Level >& level )
	{
		std::weak_ptr< Entity > emptyOwner;
		for( const std::shared_ptr< EntityRef >& entRef : level->GetLevelEntities() )
		{
			AddEntityRef( entRef, nullptr );
		}
	}

	void EntitySystem::OnLevelUnload( const std::shared_ptr< Level >& level )
	{
		for( const std::shared_ptr< EntityRef >& entRef : level->GetLevelEntities() )
		{
			RemoveEntityRef( entRef );
		}
	}

	void EntitySystem::OnInit( const GameSystemInitContext& initCtx )
	{
		initCtx.m_tickRegister.RegisterForTick( TickGroup::Game, [ this ]( const TickInfo& info )
		{
			Tick();
		} );
	}

	void EntitySystem::OnUninit()
	{
		m_refsToProcess.clear();
		ORB_ASSERT( m_registeredRefs.empty(), "There are still some entities registered" );
	}

	void EntitySystem::Tick()
	{
		Tick_ProcessCreations();
		Tick_ProcessRegisters();
		Tick_ProcessTransformUpdates();
	}

	void EntitySystem::Tick_ProcessCreations()
	{
		for( const CreateEntry& entry : m_entitiesToCreate )
		{
			AddEntity( entry );
		}
		m_entitiesToCreate.clear();
	}

	void EntitySystem::Tick_ProcessRegisters()
	{
		for( const RegisterEntry& entry : m_refsToProcess )
		{
			std::shared_ptr< EntityRef > entRef = entry.m_ref.lock();
			AddEntityRef( entRef, entry.m_callback );
		}
		m_refsToProcess.clear();
	}

	void EntitySystem::Tick_ProcessTransformUpdates()
	{
		for( const auto& it : m_transformsToUpdate )
		{
			auto entityIt = m_entities.find( it.first );
			if( entityIt == m_entities.end() )
			{
				continue;
			}

			const UpdateTransformEntry& updateEntry = it.second;

			for( const UpdateTransformEntry::ComponentData& componentData : updateEntry.m_components )
			{
				const std::shared_ptr< PositionComponent >& component = componentData.m_component.lock();
				component->Internal_SetLocalPosition( componentData.m_data.m_position );
				component->Internal_SetLocalRotation( componentData.m_data.m_rotation );
			}

			if( updateEntry.m_entityUpdate )
			{
				entityIt->second->Internal_UpdateTransformHierarchy( updateEntry.m_entityData.m_position, updateEntry.m_entityData.m_rotation );
			}
			else
			{
				entityIt->second->Internal_UpdateTransformHierarchy( updateEntry.m_entityData.m_position, updateEntry.m_entityData.m_rotation );
			}
		}

		m_transformsToUpdate.clear();
	}

	void EntitySystem::AddEntity( const CreateEntry& entry )
	{
		auto entity = AddEntity( entry.m_entityId, entry.m_resPath, entry.m_spawnPosition, entry.m_spawnRotation, entry.m_owner );
		if( entry.m_callback )
		{
			entry.m_callback( entity );
		}
	}

	void EntitySystem::AddEntityRef( const std::shared_ptr< EntityRef >& entRef, const EntityRefRegisteredCallback& callback )
	{
		auto entity = AddEntity( entRef->GetResPath(), entRef->GetPosition(), entRef->GetRotation() );
		m_registeredRefs[ entity->GetEntityID() ] = entRef;
		entRef->SetEntity( std::move( entity ) );

		if( callback )
		{
			callback( *entRef );
		}
	}

	bool EntitySystem::RemoveEntityRef( const std::shared_ptr< EntityRef >& entRef )
	{
		if( auto entity = entRef->GetEntity() )
		{
			m_registeredRefs.erase( entity->GetEntityID() );
			entity->Uninit();
			entRef->SetEntity( nullptr );
			return true;
		}

		return false;
	}

	std::shared_ptr< Entity > EntitySystem::AddEntity( const std::string& resPath, const Vector2& position, float rotation )
	{
		const EntityID entityId = ++m_entityIdGen;
		const std::weak_ptr< Entity > noOwner;
		return AddEntity( entityId, resPath, position, rotation, noOwner );
	}

	std::shared_ptr< Entity > EntitySystem::AddEntity( EntityID entityId, const std::string& resPath, const Vector2& position, float rotation, const std::weak_ptr< Entity >& owner )
	{
		auto entity = DataCenter::GetInstance().CreateSerializableFromResource< Entity >( resPath );

		EntityInitContext initCtx;
		initCtx.m_entityId = entityId;
		initCtx.m_gameInstance = GetGameInstance();
		initCtx.m_initialPosition = position;
		initCtx.m_initialRotation = rotation;
		initCtx.m_owner = owner;

		entity->Init( initCtx );

		m_entities[ entityId ] = entity;

		return entity;
	}
}