#pragma once

#include "engine/public/base/cameraSystemInterface.h"

namespace core
{
	class CameraSystem : public ICameraSystem
	{
		ORB_SUPER_CLASS( ICameraSystem );

	public:
		virtual void RegisterCamera( const std::shared_ptr< CameraComponent >& cameraComp ) override;
		virtual void UnregisterCamera( const std::shared_ptr< CameraComponent >& cameraComp ) override;
		virtual std::shared_ptr< CameraComponent > GetActiveCamera() const override;

	protected:
		virtual void OnInit( const GameSystemInitContext& initCtx ) override;
		virtual void OnUninit() override;

	private:
		void Tick();

	private:
		std::vector< std::weak_ptr< CameraComponent > > m_cameras;
	};
}