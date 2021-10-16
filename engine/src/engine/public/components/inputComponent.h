#pragma once

#include "engine/public/ecs/component.h"
#include "engine/public/inputHelpers.h"

namespace core
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class InputComponent : public Component
	{
		ORB_DECLARE_TYPE( InputComponent );

	public:
		virtual void HandleInput( const InputState& inputState ) = 0;
	};
}