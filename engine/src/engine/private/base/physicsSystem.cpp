#include "physicsSystem.h"

#include "tickRegister.h"
#include "physicsComponent.h"
#include "gameInstance.h"
#include "entitySystemInterface.h"
#include "entity.h"


ORB_REGISTER_SYSTEM( PhysicsSystem );

namespace core
{
	void PhysicsSystem::RegisterComponent( const std::shared_ptr< PhysicsComponent >& component )
	{
		m_components.push_back( { component, BoundingSphere{} , true } );
	}

	void PhysicsSystem::UnregisterComponent( const std::shared_ptr< PhysicsComponent >& component )
	{
		auto removeIt = std::remove_if( m_components.begin(), m_components.end(), [&component]( const BodyEntry& b )
		{
			return component == b.m_component.lock();
		} );

		ORB_ASSERT( removeIt != m_components.end() );
		m_components.erase( removeIt, m_components.end() );
	}

	bool PhysicsSystem::QuerySphere( const Vector2& worldPosition, float radius )
	{
		const BoundingSphere sphere{ Vector3{ worldPosition.x, worldPosition.y, 0.0f }, radius };
		for( const BodyEntry& body : m_components )
		{
			if( body.m_worldSphere.Intersects( sphere ) )
			{
				return true;
			}
		}

		return false;
	}

	void PhysicsSystem::OnInit( const GameSystemInitContext& initCtx )
	{
		Super::OnInit( initCtx );

		initCtx.m_tickRegister.RegisterForTick( TickGroup::Physics, [this]( const TickInfo& info )
		{
			Tick( info );
		} );

		m_entitySystem = &GetGameInstance()->GetSystem< IEntitySystem >();
	}

	void PhysicsSystem::OnUninit()
	{
		ORB_ASSERT( m_components.empty() );

		Super::OnUninit();
	}

	void PhysicsSystem::Tick( const TickInfo& info )
	{
		Tick_Integrate( info.GetDeltaTime() );
		Tick_CheckForCollisions();
	}

	void PhysicsSystem::Tick_Integrate( float dt )
	{
		for( BodyEntry& body : m_components )
		{
			auto physicsComponent = body.m_component.lock();
			physicsComponent->Integrate( dt );

			std::shared_ptr< Entity > owningEntity = physicsComponent->GetEntityOwner();
			const Vector2 currentPosition = owningEntity->GetPosition();
			const Vector2 newPosition = currentPosition + physicsComponent->GetVelocity() * dt;

			const float currentRotation = owningEntity->GetRotation();
			const float newRotation = currentRotation + physicsComponent->GetAngularVelocity() * dt;

			bool refreshBounds = body.m_refreshBoundingSphere;
			if( Vector2::DistanceSquared( currentPosition, newPosition ) > FLT_EPSILON
				|| !IsZero( currentRotation - newRotation ) )
			{
				m_entitySystem->SetEntityTransform( owningEntity->GetEntityID(), newPosition, newRotation );
				refreshBounds = true;		
			}

			if( refreshBounds )
			{
				BoundingSphere worldSphere;
				physicsComponent->GetBoundingSphere().Transform(
					body.m_worldSphere,
					Matrix::CreateRotationZ( newRotation ) * Matrix::CreateTranslation( newPosition.x, newPosition.y, 0.0f ) );
			}
		}
	}

	void PhysicsSystem::Tick_CheckForCollisions()
	{
		for( size_t i = 0; i < m_components.size(); ++i )
		{
			for( size_t j = i + 1; j < m_components.size(); ++j )
			{
				if( m_components[ i ].m_worldSphere.Intersects( m_components[ j ].m_worldSphere ) )
				{
					auto i_c = m_components[ i ].m_component.lock();
					auto j_c = m_components[ j ].m_component.lock();
					i_c->OnCollision( *j_c );
					j_c->OnCollision( *i_c );
				}
			}
		}
	}
}