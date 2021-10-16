#pragma once

#include <chrono>

#include "engineStage.h"

namespace core
{
	class IRenderer;
	class Level;
	class TickInfo;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct RuntimeStageInitData
	{
		HWND m_windowHandle = nullptr;
		std::weak_ptr< IRenderer > m_renderer;
		const char* m_startingLevelResPath = nullptr;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class EngineRuntimeStage : public EngineStage
	{

	private:
		enum class PumpResult
		{
			Continue,
			Exit,
		};

	public:
		EngineRuntimeStage();
		virtual ~EngineRuntimeStage();

		bool Init( const RuntimeStageInitData& initData );
		virtual void Uninit() override final;

		void MainLoop();

	protected:
		virtual bool OnInit( const RuntimeStageInitData& initData ) = 0;
		virtual void OnUninit() = 0;
		virtual void OnFrameTick( const TickInfo& tickInfo ) = 0;

		void Shutdown();

	private:
		void TickFrame();
		PumpResult ProcessMessagePump();

	private:
		HWND m_windowHandle = nullptr;
		std::weak_ptr< IRenderer > m_renderer;
		bool m_shutdown = false;

		std::chrono::steady_clock::time_point m_lastTimePoint;
	};
}