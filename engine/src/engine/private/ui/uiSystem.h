#pragma once

#include "uiSystemInterface.h"

namespace core
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class UISystem : public IUISystem
	{

	public:
		UISystem();
		virtual ~UISystem();

		virtual void OnInit( const GameSystemInitContext& initCtx ) override;

		virtual void OnLevelLoad( const std::shared_ptr< Level >& level ) override;
		virtual void OnLevelUnload( const std::shared_ptr< Level >& level ) override;

		virtual std::weak_ptr< UIRootView > AddView( const char* viewResPath ) override;
		virtual void RemoveView( const std::weak_ptr< UIRootView >& view ) override;

		virtual void HandleInput( const InputState& inputState ) override;

		virtual bool IsMenuShown() const override;

	private:
		void ClearStack();

		void UpdateState( float dt );

	private:
		std::vector< std::shared_ptr< UIRootView > > m_viewStack;
	};
}