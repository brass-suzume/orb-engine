#include "uiButton.h"

#include "rendererInterface.h"
#include "inputHelpers.h"


ORB_REGISTER_TYPE_IN_NS( core, UIButton );

namespace core
{
	UIButton::~UIButton()
	{
		ORB_ASSERT( !m_buttonClickedCallback, "Callback is still set" );
	}

	bool UIButton::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_text );
		return Super::Deserialize( data );
	}

	bool UIButton::HandleInput( const InputState& state )
	{
		if( Super::HandleInput( state ) )
		{
			return true;
		}

		if( !IsVisible() )
		{
			return false;
		}

		const MouseState& mouseState = state.m_mouseState;
		const Vector2 halfExtent = GetSize() * 0.5f;
		const Vector2 position = GetPosition();
		if( mouseState.x > position.x - halfExtent.x && mouseState.x < position.x + halfExtent.x
			&& mouseState.y > position.y - halfExtent.y && mouseState.y < position.y + halfExtent.y )
		{
			if( mouseState.leftButton )
			{
				if( m_buttonClickedCallback )
				{
					m_buttonClickedCallback();
					return true;
				}
			}
		}

		return false;
	}

	void UIButton::SetOnButtonClickedCallback( OnButtonClickedCallback&& callback )
	{
		ORB_ASSERT( !m_buttonClickedCallback, "Callback will get overwritten" );
		m_buttonClickedCallback = std::move( callback );
	}

	void UIButton::ClearOnButtonClickedCallback()
	{
		m_buttonClickedCallback = nullptr;
	}

	void UIButton::SetText( const std::string& text )
	{
		m_text = text;

		auto renderer = GetRenderer();
		if( m_textProxyId != c_invalidRenderProxyID )
		{
			renderer->UpdateText( m_textProxyId, text );
		}
	}

	void UIButton::OnInit( const UIElementInitContext& initCtx )
	{
		if( auto renderer = GetRenderer() )
		{
			{
				const Vector2 ssCoords = renderer->GetScreenSpaceCoords( GetPosition() );

				RenderQuadData quadData;
				quadData.m_quadMin = ssCoords - GetSize() * 0.5f;
				quadData.m_quadMax = ssCoords + GetSize() * 0.5f;
				quadData.m_color = m_inactiveColor;
				quadData.m_layer = RenderLayer::UI;
				quadData.m_solid = true;
				quadData.m_depth = 0.01f;
				quadData.m_visible = IsVisible();
				m_buttonProxyId = renderer->CreateQuadProxy( quadData );
			}

			{
				RenderTextData textData;
				textData.m_position = GetPosition() - GetSize() * 0.5f;
				textData.m_color = m_textColor;
				textData.m_layer = RenderLayer::UI;
				textData.m_depth = 0.0f;
				textData.m_fontSize = m_fontSize;
				textData.m_text = m_text.c_str();
				textData.m_visible = IsVisible();
				m_textProxyId = renderer->CreateTextProxy( textData );
			}

		}
	}

	void UIButton::OnUninit()
	{
		if( auto renderer = GetRenderer() )
		{
			renderer->DestroyProxy( m_buttonProxyId );
			m_buttonProxyId = c_invalidRenderProxyID;

			renderer->DestroyProxy( m_textProxyId );
			m_textProxyId = c_invalidRenderProxyID;
		}
	}

	void UIButton::OnShow()
	{
		Super::OnShow();

		auto renderer = GetRenderer();
		if( m_textProxyId != c_invalidRenderProxyID )
		{			
			renderer->SetVisibility( m_textProxyId, true );
		}

		if( m_buttonProxyId != c_invalidRenderProxyID )
		{
			renderer->SetVisibility( m_buttonProxyId, true );
		}
	}

	void UIButton::OnHide()
	{
		Super::OnHide();

		auto renderer = GetRenderer();
		if( m_textProxyId != c_invalidRenderProxyID )
		{
			renderer->SetVisibility( m_textProxyId, false );
		}

		if( m_buttonProxyId != c_invalidRenderProxyID )
		{
			renderer->SetVisibility( m_buttonProxyId, false );
		}
	}
}