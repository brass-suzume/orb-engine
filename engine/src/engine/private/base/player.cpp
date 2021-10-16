#include "player.h"

#include "gameInstance.h"
#include "uiSystemInterface.h"
#include "entity.h"
#include "inputComponent.h"


ORB_REGISTER_TYPE( Player );

namespace core
{
	void Player::Update( float dt )
	{
		HandleInput();
	}

	void Player::OnInit()
	{
		Super::OnInit();
		m_uiSystem = &GetGameInstance()->GetSystem< IUISystem >();
		SetName( "Player" );
	}

	void Player::OnUninit()
	{
		m_uiSystem = nullptr;
		Super::OnUninit();
	}

	void Player::OnControlledEntityChanged()
	{
		if( std::shared_ptr< Entity > controlledEntity = GetControlledEntity() )
		{
			m_inputComponent = controlledEntity->FindFirstComponentOfType< InputComponent >();
		}
		else
		{
			m_inputComponent.reset();
		}
	}

	void Player::HandleInput()
	{
		InputState inputState;
		inputState.m_mouseState = DirectX::Mouse::Get().GetState();
		inputState.m_keyboardState = DirectX::Keyboard::Get().GetState();

		if( m_uiSystem->IsMenuShown() )
		{
			m_uiSystem->HandleInput( inputState );
		}
		else
		{
			if( auto inputComponent = m_inputComponent.lock() )
			{
				inputComponent->HandleInput( inputState );
			}
		}
	}
}