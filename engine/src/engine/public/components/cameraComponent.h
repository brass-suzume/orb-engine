#pragma once

#include "engine/public/ecs/positionComponent.h"

namespace core
{
	class CameraComponent : public PositionComponent
	{
		ORB_DECLARE_TYPE( CameraComponent );
		ORB_SUPER_CLASS( PositionComponent );

	public:
		virtual bool Deserialize( const json& data ) override;

		Matrix GetView() const;
		Matrix GetProjection() const;

	protected:
		virtual void OnInit( const ComponentInitContext& initCtx ) override;
		virtual void OnUninit() override;
		virtual void OnTransformUpdated() override;

	protected:
		float m_ortoWidth = 64.0f;
		float m_ortoHeight = 64.0f;
		float m_ortoNear = 0.0f;
		float m_ortoFar = 1.0f;
	};
}