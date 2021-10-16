#include "meshComponent.h"

#include "resRef.h"
#include "entity.h"
#include "gameInstance.h"
#include "mesh.h"
#include "rendererInterface.h"


ORB_REGISTER_TYPE( MeshComponent );

namespace core
{
	MeshComponent::~MeshComponent()
	{
		ORB_ASSERT( !m_mesh, "Component wasn't uninitialized correctly" );
	}

	bool MeshComponent::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OBJECT_PROPERTY( m_meshRef );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_scale );
		return Super::Deserialize( data );
	}

	float MeshComponent::GetBoundingSphere() const
	{
		return m_scale * m_mesh->GetBoundingSphere();
	}

	void MeshComponent::OnInit( const ComponentInitContext& initCtx )
	{
		Super::OnInit( initCtx );

		m_mesh = DataCenter::GetInstance().CreateSerializableFromResource< Mesh >( m_meshRef->GetResPath() );

		if( auto renderer = GetEntityOwner()->GetGameInstance()->GetRenderer() )
		{
			RenderPrimitiveData pd;
			
			pd.m_layer = RenderLayer::World;
			pd.m_position = GetWorldPosition();
			pd.m_rotation = GetWorldRotation();
			pd.m_scale = m_scale;
			pd.m_vertices = m_mesh->GetVertices().data();
			pd.m_verticesCount = m_mesh->GetVertices().size();
			pd.m_indices = m_mesh->GetIndices().data();
			pd.m_indicesCount = m_mesh->GetIndices().size();
			pd.m_color = m_mesh->GetColor();

			m_meshProxyId = renderer->CreatePrimitiveProxy( pd );
		}
	}

	void MeshComponent::OnUninit()
	{
		if( auto renderer = GetEntityOwner()->GetGameInstance()->GetRenderer() )
		{
			renderer->DestroyProxy( m_meshProxyId );
			m_meshProxyId = c_invalidRenderProxyID;
		}

		m_mesh = nullptr;

		Super::OnUninit();
	}

	void MeshComponent::OnTransformUpdated()
	{
		Super::OnTransformUpdated();

		if( auto renderer = GetEntityOwner()->GetGameInstance()->GetRenderer() )
		{
			renderer->UpdateTransform( m_meshProxyId, GetWorldPosition(), GetWorldRotation() );
		}
	}
}