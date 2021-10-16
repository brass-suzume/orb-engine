#include "uiSystem.h"

#include "uiRootView.h"
#include "level.h"
#include "gameInstance.h"
#include "tickRegister.h"


ORB_REGISTER_SYSTEM( UISystem );

namespace core
{
	UISystem::UISystem() = default;

	UISystem::~UISystem()
	{
		ORB_ASSERT( m_viewStack.empty(), "View stack still contains views" );
	}

	void UISystem::OnInit( const GameSystemInitContext& initCtx )
	{
		initCtx.m_tickRegister.RegisterForTick(
			TickGroup::UI,
			[this]( const TickInfo& tickInfo ) 
			{ 
				UpdateState( tickInfo.GetDeltaTime() ); 
			}
		);
	}

	void UISystem::OnLevelLoad( const std::shared_ptr< Level >& level )
	{
		AddView( level->GetUserInterfacePath().c_str() );
	}

	void UISystem::OnLevelUnload( const std::shared_ptr< Level >& level )
	{
		ClearStack();
	}

	std::weak_ptr< UIRootView > UISystem::AddView( const char* viewResPath )
	{
		auto view = DataCenter::GetInstance().CreateSerializableFromResource< UIRootView >( viewResPath );

		m_viewStack.push_back( view );

		const UIElementInitContext renderCtx{ GetGameInstance() };
		view->Init( renderCtx );

		return view;
	}

	void UISystem::RemoveView( const std::weak_ptr< UIRootView >& view )
	{
		auto it = std::find_if( m_viewStack.begin(), m_viewStack.end(), [ viewPtr = view.lock().get() ]( const std::shared_ptr< UIRootView >& other )
		{
			return viewPtr == &*other;
		} );

		ORB_ASSERT( it != m_viewStack.end(), "View doesn't exist" );

		( *it )->Uninit();
		m_viewStack.erase( it );
	}

	void UISystem::HandleInput( const InputState& inputState )
	{
		for( auto it = m_viewStack.rbegin(); it != m_viewStack.rend(); it++ )
		{
			const std::shared_ptr< UIRootView >& view = *it;
			if( view->IsVisible() && view->HandleInput( inputState ) )
			{
				break;
			}
		}
	}

	bool UISystem::IsMenuShown() const
	{
		for( auto it = m_viewStack.rbegin(); it != m_viewStack.rend(); it++ )
		{
			const std::shared_ptr< UIRootView >& view = *it;
			if( view->IsMenuShown() )
			{
				return true;
			}
		}

		return false;
	}

	void UISystem::ClearStack()
	{
		for( const std::shared_ptr< UIRootView >& view : m_viewStack )
		{
			view->Uninit();
		}

		m_viewStack.clear();
	}

	void UISystem::UpdateState( float dt )
	{
		auto forwardIt = m_viewStack.begin();
		auto hideIt = m_viewStack.rbegin();
		for( auto it = m_viewStack.rbegin(); it != m_viewStack.rend(); it++ )
		{
			const std::shared_ptr< UIRootView >& view = *it;
			if( view->HiddenByOther() )
			{
				view->SetHiddenByOther( false );
			}

			if( view->IsVisible() )
			{
				if( view->HidesOthers() )
				{
					forwardIt = ( it + 1 ).base();
					hideIt = it + 1;
					break;
				}
			}
		}

		for( auto it = hideIt; it != m_viewStack.rend(); it++ )
		{
			const std::shared_ptr< UIRootView >& view = *it;
			if( !view->HiddenByOther() )
			{
				view->SetHiddenByOther( true );
			}
		}
	}
}