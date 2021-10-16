#pragma once

#include "controllerSystemInterface.h"

namespace core
{
	class Controller;
	class Entity;

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	class ControllerSystem : public IControllerSystem
	{

	private:
		struct Entry
		{
			std::shared_ptr< Controller > m_controller;
			std::shared_ptr< Entity > m_entity;
		};

	public:
		ControllerSystem();
		virtual ~ControllerSystem();

		virtual void CreatePlayerEntity( const std::string& resPath, const LevelSpawnPoint& spawnPoint ) override;
		virtual void DestroyPlayerEntity() override;
		virtual std::shared_ptr< Entity > GetPlayerEntity() const override;
		virtual std::shared_ptr< Controller > GetPlayerController() const override;

		virtual std::shared_ptr< Controller > RequestNewController( const std::string& controllerTypeName, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint ) override;
		virtual void ClearNonplayers() override;

		virtual void CreateControllerEntity( const std::shared_ptr< Controller >& controller, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint ) override;
		virtual void DestroyControllerEntity( const std::shared_ptr< Controller >& controller ) override;

		virtual std::shared_ptr< Controller > GetControllerForEntity( const std::shared_ptr< Entity >& entity ) const override;

		virtual ControllerEntityChangedCbReg::ID RegisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::TCallbackFunctor&& callback ) override;
		virtual void UnregisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::ID& callbackID ) override;

		virtual void GetControllerEntities( std::vector< std::shared_ptr< Entity > >& entities ) const override;

	protected:
		virtual void OnInit( const GameSystemInitContext& initCtx ) override;
		virtual void OnUninit() override;

	private:
		void AddController( const std::shared_ptr< Controller >& c );
		void SetPlayerEntity( const std::shared_ptr< Entity >& entity );
		void SetControllerEntity( const std::shared_ptr< Controller >& c, const std::shared_ptr< Entity >& entity );

		void ReleaseControl( Entry& entry );

		void UpdateControllers( float dt );

		void NotifyListeners( const std::shared_ptr< Controller >& affectedController );

	private:
		std::vector< Entry > m_controllers;
		ControllerEntityChangedCbReg m_cbRegister;
	};
}