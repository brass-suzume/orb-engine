#pragma once

#include "engine/public/base/controller.h"

namespace core
{
	class IUISystem;
	class InputComponent;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Player : public Controller
	{
		ORB_DECLARE_TYPE( Player );
		ORB_SUPER_CLASS( Controller );

	public:
		virtual void Update( float dt ) override;

	protected:
		virtual void OnInit() override;
		virtual void OnUninit() override;

		virtual void OnControlledEntityChanged() override;

	private:
		void HandleInput();

	private:
		IUISystem* m_uiSystem = nullptr;

		std::weak_ptr< InputComponent > m_inputComponent;
	};
}