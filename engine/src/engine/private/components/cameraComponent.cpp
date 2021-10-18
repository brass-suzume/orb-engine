#include "cameraComponent.h"

#include "gameInstance.h"
#include "cameraSystemInterface.h"


ORB_REGISTER_TYPE_IN_NS( core, CameraComponent );

namespace core
{
	bool CameraComponent::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_ortoWidth );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_ortoHeight );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_ortoNear );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_ortoFar );
		return Super::Deserialize( data );
	}

	Matrix CameraComponent::GetView() const
	{
		Matrix wt = GetWorldTransform();
		Matrix view;
		wt.Invert( view );
		return view;
	}

	Matrix CameraComponent::GetProjection() const
	{
		return Matrix::CreateOrthographic( m_ortoWidth, m_ortoHeight, m_ortoNear, m_ortoFar );
	}

	void CameraComponent::OnInit( const ComponentInitContext& initCtx )
	{
		Super::OnInit( initCtx );
	
		auto sharedThis = std::static_pointer_cast< CameraComponent >( shared_from_this() );
		GetGameInstance()->GetSystem< ICameraSystem >().RegisterCamera( sharedThis );
	}

	void CameraComponent::OnUninit()
	{
		auto sharedThis = std::static_pointer_cast< CameraComponent >( shared_from_this() );
		GetGameInstance()->GetSystem< ICameraSystem >().UnregisterCamera( sharedThis );

		Super::OnUninit();
	}

	void CameraComponent::OnTransformUpdated()
	{
		Super::OnTransformUpdated();
	}
}