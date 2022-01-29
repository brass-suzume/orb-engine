#include "entity.h"

#include "component.h"
#include "entityRef.h"
#include "positionComponent.h"


ORB_REGISTER_TYPE_IN_NS( core, Entity );

namespace core
{
	Entity::~Entity()
	{
		ORB_ASSERT( m_entityId == c_invalidEntityID, "Destroying initialized entity" );
	}

	void Entity::Init( const EntityInitContext& initCtx )
	{
		ORB_ASSERT( m_entityId == c_invalidEntityID, "Entity id already assigned" );

		m_entityId = initCtx.m_entityId;
		m_owner = initCtx.m_owner;
		m_gameInstance = initCtx.m_gameInstance;

		m_position = initCtx.m_initialPosition;
		m_rotation = initCtx.m_initialRotation;

		for( const std::shared_ptr< Component >& c : m_components )
		{
			c->GatherComponents( m_allComponents );
		}

		OnInit( initCtx );

		const ComponentInitContext compInitCtx{ shared_from_this() };
		for( const std::shared_ptr< Component >& c : m_components )
		{
			if( auto positionComp = std::dynamic_pointer_cast< PositionComponent >( c ) )
			{
				m_positionComponents.push_back( positionComp );
			}
			c->Init( compInitCtx );
		}

		Internal_RefreshHierarchy();

		for( const std::shared_ptr< Component >& c : m_allComponents )
		{
			c->PostInit();
		}
	}

	void Entity::Uninit()
	{
		for( const std::shared_ptr< Component >& c : m_components )
		{
			c->Uninit();
		}

		OnUninit();

		m_allComponents.clear();
		m_positionComponents.clear();

		m_gameInstance = nullptr;
		m_owner.reset();
		m_entityId = c_invalidEntityID;
	}

	Matrix Entity::GetWorldTransform() const
	{
		return Matrix::CreateRotationZ( m_rotation ) * Matrix::CreateTranslation( m_position.x, m_position.y, 0 );
	}

	Vector2 Entity::GetForward() const
	{
		return Vector2{ GetWorldTransform().Up() };
	}

	Vector2 Entity::GetRight() const
	{
		return Vector2{ GetWorldTransform().Right() };
	}

	bool Entity::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OBJECT_ARRAY_PROPERTY( m_components );
		return Super::Deserialize( data );
	}

	void Entity::OnInit( const EntityInitContext& initCtx )
	{
	}

	void Entity::OnUninit()
	{
	}

	void Entity::Internal_RefreshHierarchy()
	{
		const Matrix worldTransform = GetWorldTransform();
		for( const std::shared_ptr< PositionComponent >& comp : m_positionComponents )
		{
			comp->Internal_PropagateTransform( worldTransform );
		}
	}

	void Entity::Internal_UpdateTransformHierarchy( const Vector2& position, float rotation )
	{
		m_position = position;
		m_rotation = rotation;

		Internal_RefreshHierarchy();
	}
}