#include "engineWindowInitStage.h"

namespace core
{
	namespace
	{
		const char* s_windowClassName = "MAIN";

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		LRESULT CALLBACK WindowProc(
			HWND   hwnd,
			UINT   uMsg,
			WPARAM wParam,
			LPARAM lParam
		)
		{
			if( uMsg == WM_CLOSE || uMsg == WM_DESTROY )
			{
				PostQuitMessage( 0 );
				return 0;
			}

			switch( uMsg )
			{
			case WM_ACTIVATEAPP:
			case WM_INPUT:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_MOUSEHOVER:
				DirectX::Mouse::ProcessMessage( uMsg, wParam, lParam );
				break;
			}

			switch( uMsg )
			{
			case WM_ACTIVATEAPP:
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
				DirectX::Keyboard::ProcessMessage( uMsg, wParam, lParam );
				break;
			}

			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//

	EngineWindowInitStage::~EngineWindowInitStage()
	{
		ORB_ASSERT( !m_windowClassRegistered, "Window class still registered" );
		ORB_ASSERT( m_windowHandle == nullptr, "Failed to clear wnd handle" );
		ORB_ASSERT( m_windowModule == nullptr, "Failed to clear wnd module" );
	}

	bool EngineWindowInitStage::Init( const WindowInitData& initData )
	{
		HMODULE moduleHandle = GetModuleHandle( nullptr );
		if( !moduleHandle )
		{
			return false;
		}

		m_windowModule = moduleHandle;

		WNDCLASSEX wndClass;
		ZeroMemory( &wndClass, sizeof( WNDCLASSEX ) );
		wndClass.cbSize = sizeof( WNDCLASSEX );
		wndClass.lpszClassName = s_windowClassName;
		wndClass.hInstance = moduleHandle;
		wndClass.lpfnWndProc = WindowProc;

		ATOM atom = RegisterClassEx( &wndClass );
		if( atom == 0 )
		{
			return false;
		}
		m_windowClassRegistered = true;

		HWND wndHandle = CreateWindowEx( WS_EX_OVERLAPPEDWINDOW, s_windowClassName, initData.m_windowName,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900, nullptr, nullptr, moduleHandle, nullptr );
		if( !wndHandle )
		{
			return false;
		}

		ShowWindow( wndHandle, SW_SHOW );
		UpdateWindow( wndHandle );

		m_windowHandle = wndHandle;
		m_mouse = std::make_unique<DirectX::Mouse>();
		m_keyboard = std::make_unique<DirectX::Keyboard>();

		return true;
	}

	void EngineWindowInitStage::Uninit()
	{
		m_keyboard.reset();
		m_mouse.reset();

		if( m_windowHandle )
		{
			DestroyWindow( m_windowHandle );
			m_windowHandle = nullptr;
		}

		if( m_windowClassRegistered )
		{
			HMODULE handle = GetModuleHandle( nullptr );
			UnregisterClass( s_windowClassName, handle );
			m_windowClassRegistered = false;
		}

		m_windowModule = nullptr;
	}
}