#pragma once

#include "engine/public/base/gameSystem.h"

namespace core
{
	class Controller;
	class EntityRef;
	struct LevelSpawnPoint;
	class Entity;

	class IControllerSystem : public TGameSystem< IControllerSystem >
	{

	public:
		virtual void CreatePlayerEntity( const std::string& resPath, const LevelSpawnPoint& spawnPoint ) = 0;
		virtual void DestroyPlayerEntity() = 0;
		virtual std::shared_ptr< Entity > GetPlayerEntity() const = 0;
		virtual std::shared_ptr< Controller > GetPlayerController() const = 0;

		virtual std::shared_ptr< Controller > RequestNewController( const std::string& controllerTypeName, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint ) = 0;
		virtual void ClearNonplayers() = 0;

		virtual std::shared_ptr< Controller > GetControllerForEntity( const std::shared_ptr< Entity >& entity ) const = 0;

		virtual ControllerEntityChangedCbReg::ID RegisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::TCallbackFunctor&& callback ) = 0;
		virtual void UnregisterControlledEntityChangedCallback( ControllerEntityChangedCbReg::ID& callbackID ) = 0;

		virtual void CreateControllerEntity( const std::shared_ptr< Controller >& controller, const std::string& botEntityResPath, const LevelSpawnPoint& spawnPoint ) = 0;
		virtual void DestroyControllerEntity( const std::shared_ptr< Controller >& controller ) = 0;

		virtual void GetControllerEntities( std::vector< std::shared_ptr< Entity > >& entities ) const = 0;

		template< class TController >
		std::shared_ptr< TController > RequestNewController( const std::string& controllerRef, const std::string& entityRef )
		{
			return std::dynamic_pointer_cast< TController >( RequestNewController( controllerRef, entityRef ) );
		}
	};
}