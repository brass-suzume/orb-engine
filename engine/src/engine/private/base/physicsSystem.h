#pragma once

#include "physicsSystemInterface.h"

namespace core
{
	class IEntitySystem;
	class TickInfo;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	class PhysicsSystem : public IPhysicsSystem
	{
		ORB_SUPER_CLASS( IPhysicsSystem );

	private:
		struct BodyEntry
		{
			std::weak_ptr< PhysicsComponent > m_component;
			BoundingSphere m_worldSphere;
			bool m_refreshBoundingSphere = false;
		};

	public:
		virtual void RegisterComponent( const std::shared_ptr< PhysicsComponent >& component ) override;
		virtual void UnregisterComponent( const std::shared_ptr< PhysicsComponent >& component ) override;

		virtual bool QuerySphere( const Vector2& worldPosition, float radius ) override;

	protected:
		virtual void OnInit( const GameSystemInitContext& initCtx ) override;
		virtual void OnUninit() override;

	private:
		void Tick( const TickInfo& info );
		void Tick_Integrate( float dt );
		void Tick_CheckForCollisions();

	private:
		IEntitySystem* m_entitySystem = nullptr;
		std::vector< BodyEntry > m_components;
	};
}