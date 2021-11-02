#pragma once

#include "engine/public/base/gameSystem.h"

namespace core
{
	class PhysicsComponent;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class IPhysicsSystem : public TGameSystem< IPhysicsSystem >
	{

	public:
		virtual void RegisterComponent( const std::shared_ptr< PhysicsComponent >& component ) = 0;
		virtual void UnregisterComponent( const std::shared_ptr< PhysicsComponent >& component ) = 0;

		virtual bool QuerySphere( const Vector2& worldPosition, float radius ) = 0;
	};
}