#include "engineRenderingInitStage.h"

#include "renderer.h"

namespace core
{
	EngineRenderingInitStage::~EngineRenderingInitStage()
	{
		ORB_ASSERT( !m_renderer, "Renderer is still present" );
	}

	bool EngineRenderingInitStage::Init( const RenderingInitData& initData )
	{
		m_renderer = std::make_shared<Renderer>();
		return m_renderer->Init( initData.m_windowHandle );
	}

	void EngineRenderingInitStage::Uninit()
	{
		m_renderer->Uninit();
		ORB_ASSERT( m_renderer.use_count() == 1, "Renderer reference is still stored somewhere" );
		m_renderer.reset();
	}
}