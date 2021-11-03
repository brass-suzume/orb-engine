#include "gameSystem.h"

namespace core
{
	namespace prv
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		SystemID GenerateID()
		{
			static SystemID s_generator = 0;
			return ++s_generator;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	GameSystemRegistrar& GameSystemRegistrar::GetInstance()
	{
		static GameSystemRegistrar s_instance;
		return s_instance;
	}

	GameSystemRegistrar::GameSystemRegistrar() = default;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	void IGameSystem::Init( const GameSystemInitContext& initCtx )
	{
		m_gameInstance = &initCtx.m_gameInstance;
		OnInit( initCtx );
	}

	void IGameSystem::Uninit()
	{
		OnUninit();
		m_gameInstance = nullptr;
	}

	void IGameSystem::OnLevelLoad( const std::shared_ptr< Level >& level )
	{
	}

	void IGameSystem::OnLevelUnload( const std::shared_ptr< Level >& level )
	{
	}

	void IGameSystem::OnInit( const GameSystemInitContext& initCtx )
	{
	}

	void IGameSystem::OnUninit()
	{
	}
}