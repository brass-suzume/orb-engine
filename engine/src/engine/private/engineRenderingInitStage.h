#pragma once

#include "engineStage.h"

namespace core
{
	class Renderer;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct RenderingInitData
	{
		HMODULE m_windowModule;
		HWND m_windowHandle;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class EngineRenderingInitStage final : public EngineStage
	{

	public:
		virtual ~EngineRenderingInitStage() override;

		bool Init( const RenderingInitData& initData );
		virtual void Uninit() override;

		std::shared_ptr< Renderer > GetRenderer() const { return m_renderer; }

	private:
		std::shared_ptr< Renderer > m_renderer;
	};
}