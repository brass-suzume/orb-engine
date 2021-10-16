#pragma once

#include "engine/public/ui/elements/uiElement.h"

namespace core
{
	using OnButtonClickedCallback = std::function< void() >;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class UIButton : public UIElement
	{
		ORB_DECLARE_TYPE( UIButton );
		ORB_SUPER_CLASS( UIElement );

	public:
		virtual ~UIButton();

		virtual bool Deserialize( const json& data ) override;
		virtual bool HandleInput( const InputState& state ) override;

		void SetOnButtonClickedCallback( OnButtonClickedCallback&& callback );
		void ClearOnButtonClickedCallback();

		void SetText( const std::string& text );
		
	protected:
		virtual void OnInit( const UIElementInitContext& initCtx ) override;
		virtual void OnUninit() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		Color GetCurrentColor() const;

	private:
		OnButtonClickedCallback m_buttonClickedCallback;
		RenderProxyID m_buttonProxyId = c_invalidRenderProxyID;
		RenderProxyID m_textProxyId = c_invalidRenderProxyID;

		std::string m_text;
		Color m_inactiveColor = Colors::Gray;
		Color m_hoverColor = Colors::LightYellow;
		Color m_textColor = Colors::Black;
		float m_fontSize = 16.0f;

	};
}