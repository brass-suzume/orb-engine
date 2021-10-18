#pragma once

#include "engine/public/base/gameSystem.h"

namespace core
{
	class UIRootView;
	class InputState;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	class IUISystem : public TGameSystem< IUISystem >
	{

	public:
		virtual std::weak_ptr< UIRootView > AddView( const std::string& viewResPath ) = 0;
		virtual void RemoveView( const std::weak_ptr< UIRootView >& view ) = 0;

		virtual void HandleInput( const InputState& inputState ) = 0;

		virtual bool IsMenuShown() const = 0;
	};
}