#pragma once

#include "engine/public/base/gameSystem.h"

namespace core
{
	class CameraComponent;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class ICameraSystem : public TGameSystem< ICameraSystem >
	{

	public:
		virtual void RegisterCamera( const std::shared_ptr< CameraComponent >& cameraComp ) = 0;
		virtual void UnregisterCamera( const std::shared_ptr< CameraComponent >& cameraComp ) = 0;
		virtual std::shared_ptr< CameraComponent > GetActiveCamera() const = 0;
	};
}