#include "controller.h"


ORB_REGISTER_ABSTRACT_TYPE_IN_NS( core, Controller );

namespace core
{
	Controller::~Controller()
	{
		ORB_ASSERT( m_controlledEntity.expired(), "Controller still controlling an entity on dtor" );
	}

	void Controller::Init( const GameInstance& gameInstance )
	{
		m_gameInstance = &gameInstance;
		OnInit();
	}

	void Controller::Uninit()
	{
		OnUninit();
		m_gameInstance = nullptr;
	}

	void Controller::SetControlledEntity( const std::shared_ptr<Entity>& entity )
	{
		m_controlledEntity = entity;
		OnControlledEntityChanged();
	}

	void Controller::OnInit()
	{
	}

	void Controller::OnUninit()
	{
	}

	void Controller::OnControlledEntityChanged()
	{
	}
}