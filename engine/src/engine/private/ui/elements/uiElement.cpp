#include "elements/uiElement.h"

#include "gameInstance.h"


ORB_REGISTER_TYPE_IN_NS( core, UIElement );

namespace core
{
	UIElement::~UIElement()
	{
		ORB_ASSERT( !m_isInitialized, "Failed to uninitialize the ui element" );
	}

	bool UIElement::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_OPTIONAL_PROPERTY( m_isVisible );
		ORB_DESERIALIZE_PROPERTY( m_position );
		ORB_DESERIALIZE_PROPERTY( m_size );
		return Super::Deserialize( data );
	}

	bool UIElement::HandleInput( const InputState& state )
	{
		return false;
	}

	void UIElement::Init( const UIElementInitContext& initCtx )
	{
		m_isInitialized = true;
		m_desiredVisibility = m_isVisible;
		m_gameInstance = initCtx.m_gameInstance;
		OnInit( initCtx );
	}

	void UIElement::Uninit()
	{
		OnUninit();
		m_gameInstance = nullptr;
		m_isInitialized = false;
	}

	void UIElement::SetDesiredVisibility( bool isVisible )
	{
		m_desiredVisibility = isVisible;
		EvaluateVisibility();
	}

	std::shared_ptr< IRenderer > UIElement::GetRenderer() const
	{
		return m_gameInstance->GetRenderer();
	}

	void UIElement::OnInit( const UIElementInitContext& initCtx )
	{
	}

	void UIElement::OnUninit()
	{
	}

	bool UIElement::HasDesireForVisibility() const
	{
		return m_desiredVisibility;
	}

	void UIElement::EvaluateVisibility()
	{
		const bool wantsVisibility = HasDesireForVisibility();
		if( m_isVisible != wantsVisibility )
		{
			m_isVisible = wantsVisibility;
			if( wantsVisibility )
			{
				OnShow();
			}
			else
			{
				OnHide();
			}
		}
	}

	void UIElement::OnShow()
	{
	}

	void UIElement::OnHide()
	{
	}
}