#pragma once

#include "engine/public/ui/elements/uiElement.h"

namespace core
{
	class UIRootView : public UIElement
	{
		ORB_DECLARE_TYPE( UIRootView );
		ORB_SUPER_CLASS( UIElement );

	public:
		virtual bool Deserialize( const json& data ) override;

		bool HidesOthers() const { return m_hidesOthers; }

		bool HiddenByOther() const { return m_hiddenByOther; }
		void SetHiddenByOther( bool hiddenByOther );

		virtual bool IsMenuShown() const { return false; }

	protected:
		virtual bool HasDesireForVisibility() const override;

	private:
		bool m_hidesOthers = false;
		bool m_hiddenByOther = false;
	};
}