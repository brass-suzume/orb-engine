#include "engineRuntimeStage.h"

#include "tickRegister.h"
#include "rendererInterface.h"


namespace core
{
	namespace
	{
		const float c_maxTimestep = 1.0f / 60.0f;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	TickInfo::TickInfo( float dt )
		: m_dt{ dt }
	{
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	EngineRuntimeStage::EngineRuntimeStage() = default;

	EngineRuntimeStage::~EngineRuntimeStage()
	{
		ORB_ASSERT( !m_windowHandle, "Window handle still exists" );
	}

	bool EngineRuntimeStage::Init( const RuntimeStageInitData& initData )
	{
		srand( time( NULL ) );

		m_windowHandle = initData.m_windowHandle;
		m_renderer = initData.m_renderer;
		m_lastTimePoint = std::chrono::high_resolution_clock::now();
		return OnInit( initData );
	}

	void EngineRuntimeStage::Uninit()
	{
		OnUninit();
		m_windowHandle = nullptr;
	}

	void EngineRuntimeStage::MainLoop()
	{
		while( !m_shutdown )
		{
			if( ProcessMessagePump() == PumpResult::Continue )
			{
				TickFrame();
				
				if( auto renderer = m_renderer.lock() )
				{
					renderer->Render();
				}
			}
			else
			{
				m_shutdown = true;
			}
		}
	}

	void EngineRuntimeStage::Shutdown()
	{
		m_shutdown = true;
	}

	void EngineRuntimeStage::TickFrame()
	{
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const uint64_t nanoDt = std::chrono::duration_cast< std::chrono::nanoseconds >( currentTime - m_lastTimePoint ).count(); 
		const float dt = nanoDt * 1e-9;
		const float clampedDt = min( dt, c_maxTimestep );
		const TickInfo tickInfo{ clampedDt };
		OnFrameTick( tickInfo );
		m_lastTimePoint = currentTime;
	}

	EngineRuntimeStage::PumpResult EngineRuntimeStage::ProcessMessagePump()
	{
		MSG msg;

		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if( msg.message == WM_QUIT )
			{
				return PumpResult::Exit;
			}
		}

		return PumpResult::Continue;
	}
}