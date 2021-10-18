#include "physicsComponent.h"

#include "gameInstance.h"
#include "physicsSystemInterface.h"
#include "entity.h"
#include "meshComponent.h"
#include "mesh.h"


ORB_REGISTER_TYPE_IN_NS( core, PhysicsComponent );

namespace core
{
	bool PhysicsComponent::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_mass );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_maxVelocity );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_maxAngularVelocity );
		return Super::Deserialize( data );
	}

	void PhysicsComponent::PostInit()
	{
		Super::PostInit();
		GetEntityOwner()->ForEachComponentOfType< MeshComponent >( [ this ]( const std::shared_ptr< MeshComponent >& meshComp )
		{
			const float meshBoundingSphereRadius = meshComp->GetBoundingSphere();
			const Vector2 meshLocalPosition = meshComp->GetLocalPosition();
			const BoundingSphere meshBoundingSphere{ Vector3{ meshLocalPosition.x, meshLocalPosition.y, 0 }, meshBoundingSphereRadius };
			BoundingSphere merged;
			BoundingSphere::CreateMerged( merged, m_boundingSphere, meshBoundingSphere );
			m_boundingSphere = merged;
		} );
	}

	void PhysicsComponent::Integrate( float dt )
	{
		m_velocity += m_acceleration * dt;
		m_acceleration = Vector2::Zero;

		if( m_maxVelocity > 0.0f )
		{
			if( m_velocity.LengthSquared() > m_maxVelocity * m_maxVelocity )
			{
				m_velocity.Normalize();
				m_velocity *= m_maxVelocity;
			}
		}

		m_angularVelocity += m_angularAcceleration * dt;
		m_angularAcceleration = 0.0f;

		if( m_maxAngularVelocity > 0.0f )
		{
			m_angularVelocity = min( max( m_angularVelocity, -m_maxAngularVelocity ), m_maxAngularVelocity );
		}
	}

	void PhysicsComponent::AddForce( const Vector2& force )
	{
		m_acceleration += force / m_mass;
	}

	void PhysicsComponent::AddTorque( float torque )
	{
		m_angularAcceleration += torque / m_mass;
	}

	CollisionCbReg::ID PhysicsComponent::RegisterCollisionCallback( CollisionCbReg::TCallbackFunctor&& callback )
	{
		return m_callbackRegistry.RegisterCallback( std::move( callback ) );
	}

	void PhysicsComponent::UnregisterCollisionCallback( CollisionCbReg::ID& callbackId )
	{
		m_callbackRegistry.UnregisterCallback( callbackId );
	}

	void PhysicsComponent::OnCollision( const PhysicsComponent& other )
	{
		m_callbackRegistry.Notify( other );
	}

	void PhysicsComponent::OnInit( const ComponentInitContext& initCtx )
	{
		Super::OnInit( initCtx );

		auto sharedThis = std::static_pointer_cast< PhysicsComponent >( shared_from_this() );
		GetGameInstance()->GetSystem< IPhysicsSystem >().RegisterComponent( sharedThis );
	}

	void PhysicsComponent::OnUninit()
	{
		auto sharedThis = std::static_pointer_cast< PhysicsComponent >( shared_from_this() );
		GetGameInstance()->GetSystem< IPhysicsSystem >().UnregisterComponent( sharedThis );

		Super::OnUninit();
	}
}