#include "engine.h"

#include "engineWindowInitStage.h"
#include "engineRenderingInitStage.h"
#include "engineRuntimeStage.h"
#include "engineInitData.h"

#include "renderer.h"


namespace core
{
	Engine::Engine()
	{

	}

	Engine::~Engine()
	{

	}

	void Engine::Start( EngineInitData&& initData )
	{
		if( Initialize( std::move( initData ) ) )
		{
			RunEngine();
		}
		Uninitialize();
	}

	bool Engine::Initialize( EngineInitData&& initData )
	{
		WindowInitData windowInitData;
		windowInitData.m_windowName = initData.m_windowName;
		auto* windowStage = AddStage< EngineWindowInitStage >( windowInitData );
		if( !windowStage )
		{
			return false;
		}

		RenderingInitData renderingInitData;
		renderingInitData.m_windowModule = windowStage->GetWindowModule();
		renderingInitData.m_windowHandle = windowStage->GetWindowHandle();
		auto* renderingStage = AddStage< EngineRenderingInitStage >( renderingInitData );
		if( !renderingStage )
		{
			return false;
		}

		RuntimeStageInitData runtimeInitData;
		runtimeInitData.m_windowHandle = windowStage->GetWindowHandle();
		runtimeInitData.m_renderer = renderingStage->GetRenderer();
		runtimeInitData.m_startingLevelResPath = initData.m_startingLevelResPath;
		m_runtimeStage = AddStage( std::move( initData.m_runtimeStage ), runtimeInitData );

		return m_runtimeStage != nullptr;
	}

	void Engine::Uninitialize()
	{
		m_runtimeStage = nullptr;

		while( !m_engineStages.empty() )
		{
			m_engineStages.back()->Uninit();
			m_engineStages.pop_back();
		}
	}

	void Engine::RunEngine()
	{
		m_runtimeStage->MainLoop();
	}

	template< class TStage, class TInitData >
	TStage* Engine::AddStage( std::unique_ptr< TStage >&& stage, const TInitData& initData )
	{
		if( stage->Init( initData ) )
		{
			m_engineStages.push_back( std::forward< std::unique_ptr< TStage > >( stage ) );
			return dynamic_cast< TStage* >( m_engineStages.back().get() );
		}
		else
		{
			stage->Uninit();
			return nullptr;
		}
	}

	template< class TStage, class TInitData >
	TStage* Engine::AddStage( const TInitData& initData )
	{
		return AddStage< TStage >( std::make_unique< TStage >(), initData );
	}
}