#include "uiRootView.h"


ORB_REGISTER_TYPE( UIRootView );

namespace core
{
	bool UIRootView::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_hidesOthers );
		return Super::Deserialize( data );
	}

	void UIRootView::SetHiddenByOther( bool hiddenByOther )
	{
		if( hiddenByOther != m_hiddenByOther )
		{
			m_hiddenByOther = hiddenByOther;
			EvaluateVisibility();
		}
	}

	bool UIRootView::HasDesireForVisibility() const
	{
		return Super::HasDesireForVisibility() && !m_hiddenByOther;
	}
}