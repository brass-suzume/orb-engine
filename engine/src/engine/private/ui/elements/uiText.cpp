#include "uiText.h"

#include "rendererInterface.h"


ORB_REGISTER_TYPE_IN_NS( core, UIText );

namespace core
{
	bool UIText::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_text );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_fontSize );
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_color );
		return Super::Deserialize( data );
	}

	void UIText::SetText( const std::string& text )
	{
		m_text = text;

		auto renderer = GetRenderer();
		if( m_proxyId != c_invalidRenderProxyID )
		{
			renderer->UpdateText( m_proxyId, text );
		}
	}

	void UIText::OnInit( const UIElementInitContext& initCtx )
	{
		if( auto renderer = GetRenderer() )
		{
			RenderTextData textData;
			textData.m_position = GetPosition();
			textData.m_color = m_color;
			textData.m_layer = RenderLayer::UI;
			textData.m_depth = 0.0f;
			textData.m_fontSize = m_fontSize;
			textData.m_text = m_text.c_str();
			textData.m_visible = IsVisible();
			m_proxyId = renderer->CreateTextProxy( textData );
		}
	}

	void UIText::OnUninit()
	{
		if( auto renderer = GetRenderer() )
		{
			renderer->DestroyProxy( m_proxyId );
			m_proxyId = c_invalidRenderProxyID;
		}
	}

	void UIText::OnShow()
	{
		Super::OnShow();

		if( m_proxyId != c_invalidRenderProxyID )
		{
			auto renderer = GetRenderer();
			renderer->SetVisibility( m_proxyId, true );
		}
	}

	void UIText::OnHide()
	{
		Super::OnHide();

		if( m_proxyId != c_invalidRenderProxyID )
		{
			auto renderer = GetRenderer();
			renderer->SetVisibility( m_proxyId, false );
		}
	}
}