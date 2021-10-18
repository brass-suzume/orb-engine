#include "positionComponent.h"


ORB_REGISTER_TYPE_IN_NS( core, PositionComponent );

namespace core
{
	bool PositionComponent::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_localPosition );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_localRotation );
		ORB_DESERIALIZE_OBJECT_ARRAY_PROPERTY( m_childComponents );
		return Super::Deserialize( data );
	}

	void PositionComponent::GatherComponents( std::vector< std::shared_ptr< Component > >& components )
	{
		Super::GatherComponents( components );
		for( const std::shared_ptr< PositionComponent >& c : m_childComponents )
		{
			c->GatherComponents( components );
		}
	}

	Matrix PositionComponent::GetWorldTransform() const
	{
		return Matrix::CreateRotationZ( m_worldRotation ) * Matrix::CreateTranslation( m_worldPosition.x, m_worldPosition.y, 0.0f );
	}

	void PositionComponent::OnInit( const ComponentInitContext& initCtx )
	{
		Super::OnInit( initCtx );

		for( const std::shared_ptr< PositionComponent >& c : m_childComponents )
		{
			c->Init( initCtx );
		}
	}

	void PositionComponent::OnUninit()
	{
		for( const std::shared_ptr< PositionComponent >& c : m_childComponents )
		{
			c->Uninit();
		}

		Super::OnUninit();
	}

	void PositionComponent::OnTransformUpdated()
	{
	}

	void PositionComponent::Internal_PropagateTransform( const Matrix& parentTransform )
	{
		m_worldPosition = Vector2::Transform( m_localPosition, parentTransform );
		auto worldRot = Matrix::CreateRotationZ( m_localRotation ) * parentTransform;
		m_worldRotation = GetRoll( worldRot );

		const Matrix worldTransform = GetWorldTransform();
		for( const std::shared_ptr< PositionComponent >& c : m_childComponents )
		{
			c->Internal_PropagateTransform( worldTransform );
		}

		OnTransformUpdated();
	}
}