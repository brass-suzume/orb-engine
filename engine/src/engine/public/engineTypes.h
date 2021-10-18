#pragma once

#include <SimpleMath.h>

#include "core/public/callbackRegistry.h"

namespace core
{
	using EntityID = uint16_t;
	constexpr EntityID c_invalidEntityID = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class PhysicsComponent;
	class Controller;

	class CollisionCbReg : public CallbackRegistry< void( const PhysicsComponent& other ) > {};
	class ControllerEntityChangedCbReg : public CallbackRegistry< void( const std::shared_ptr< Controller >& ) > {};

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct LevelSpawnPoint
	{
		DirectX::SimpleMath::Vector2 m_position;
		float m_rotation = 0.0f;
	};
}