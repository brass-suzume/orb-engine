#pragma once

#include "engine/public/ecs/component.h"

namespace core
{
	class PositionComponent : public Component
	{
		ORB_DECLARE_TYPE( PositionComponent );
		ORB_SUPER_CLASS( Component );

	private:
		friend class EntitySystem;
		friend class Entity;

	public:
		virtual bool Deserialize( const json& data ) override;

		virtual void GatherComponents( std::vector< std::shared_ptr< Component > >& components );

		Vector2 GetWorldPosition() const { return m_worldPosition; }
		float GetWorldRotation() const { return m_worldRotation; }

		Vector2 GetLocalPosition() const { return m_localPosition; }
		float GetLocalRotation() const { return m_localRotation; }

		Matrix GetWorldTransform() const;

	protected:
		virtual void OnInit( const ComponentInitContext& initCtx ) override;
		virtual void OnUninit() override;

		virtual void OnTransformUpdated();

	private:
		void Internal_PropagateTransform( const Matrix& parentTransform );
		void Internal_SetLocalPosition( const Vector2& position ) { m_localPosition = position; }
		void Internal_SetLocalRotation( float rotation ) { m_localRotation = rotation; }

	private:
		std::vector< std::shared_ptr< PositionComponent > > m_childComponents;
		Vector2 m_localPosition;
		float m_localRotation = 0.0f;

		Vector2 m_worldPosition;
		float m_worldRotation = 0.0f;
	};
}