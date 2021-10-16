#pragma once

#include "engine/public/ui/elements/uiElement.h"

namespace core
{
	class UIText : public UIElement
	{
		ORB_DECLARE_TYPE( UIText );
		ORB_SUPER_CLASS( UIElement );

	public:
		virtual bool Deserialize( const json& data ) override;

		void SetText( const std::string& text );
		void SetFontSize( unsigned int fontSize );

		const std::string& GetText() const { return m_text; }
		uint8_t GetFontSize() const { return m_fontSize; }

	protected:
		virtual void OnInit( const UIElementInitContext& initCtx ) override;
		virtual void OnUninit() override;

		virtual void OnShow() override;
		virtual void OnHide() override;

	private:
		RenderProxyID m_proxyId;

		std::string m_text;
		uint8_t m_fontSize = 16.0f;
		Color m_color = Colors::White;
	};
}