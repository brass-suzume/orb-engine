#include "cameraSystem.h"

#include "tickRegister.h"
#include "gameInstance.h"
#include "rendererInterface.h"
#include "cameraComponent.h"


ORB_REGISTER_SYSTEM( CameraSystem );

namespace core
{
	void CameraSystem::RegisterCamera( const std::shared_ptr< CameraComponent >& cameraComp )
	{
		m_cameras.push_back( cameraComp );
	}

	void CameraSystem::UnregisterCamera( const std::shared_ptr< CameraComponent >& cameraComp )
	{
		auto findIt = std::find_if( m_cameras.begin(), m_cameras.end(), [ &cameraComp ]( const std::weak_ptr< CameraComponent >& c )
		{
			return c.lock() == cameraComp;
		} );

		ORB_ASSERT( findIt != m_cameras.end() );
		m_cameras.erase( findIt );
	}

	std::shared_ptr< CameraComponent > CameraSystem::GetActiveCamera() const
	{
		if( !m_cameras.empty() )
		{
			return m_cameras[ 0 ].lock();
		}

		return nullptr;
	}

	void CameraSystem::OnInit( const GameSystemInitContext& initCtx )
	{
		initCtx.m_tickRegister.RegisterForTick( TickGroup::Game, [ this ]( const TickInfo& info )
		{
			Tick();
		} );
	}

	void CameraSystem::OnUninit()
	{
		ORB_ASSERT( m_cameras.empty() );
	}

	void CameraSystem::Tick()
	{
#ifdef _DEBUG
		for( const std::weak_ptr< CameraComponent >& camera : m_cameras )
		{
			ORB_ASSERT( !camera.expired() );
		}
#endif

		if( !m_cameras.empty() )
		{
			const std::shared_ptr< CameraComponent > mainCamera = m_cameras[ 0 ].lock();
			const std::shared_ptr< IRenderer > renderer = GetGameInstance()->GetRenderer();
			renderer->SetView( mainCamera->GetView() );
			renderer->SetProjection( mainCamera->GetProjection() );
		}
	}
}