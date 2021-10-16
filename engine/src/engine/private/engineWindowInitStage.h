#pragma once

#include "engineStage.h"

namespace core
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct WindowInitData
	{
		const char* m_windowName = nullptr;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class EngineWindowInitStage final : public EngineStage
	{

	public:
		virtual ~EngineWindowInitStage() override;

		bool Init( const WindowInitData& initData );
		virtual void Uninit() override;

		HMODULE GetWindowModule() const { return m_windowModule; }
		HWND GetWindowHandle() const { return m_windowHandle; }

	private:
		bool m_windowClassRegistered = false;
		HMODULE m_windowModule = nullptr;
		HWND m_windowHandle = nullptr;

		std::unique_ptr< DirectX::Mouse > m_mouse;
		std::unique_ptr< DirectX::Keyboard > m_keyboard;
	};
}