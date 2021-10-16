#pragma once

#include "engine/public/base/gameSystem.h"

namespace core
{
	class EntityRef;
	class PositionComponent;
	class Entity;

	using EntityCreatedCallback = std::function< void( const std::shared_ptr< Entity >& entity )>;
	using EntityRefRegisteredCallback = std::function< void( const EntityRef& ) >;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class IEntitySystem : public TGameSystem< IEntitySystem >
	{

	public:
		virtual EntityID CreateEntity( const std::string& resPath, const Vector2& position, float rotation, EntityCreatedCallback&& callback, const std::shared_ptr< Entity >& owner ) = 0;
		virtual void DestroyEntity( EntityID entityId ) = 0;

		virtual void RegisterEntityRef( const std::shared_ptr< EntityRef >& ref, EntityRefRegisteredCallback&& callback ) = 0;
		virtual void UnregisterEntityRef( const std::shared_ptr< EntityRef >& ref ) = 0;

		virtual void SetEntityTransform( EntityID entityId, const Vector2& position, float rotation ) = 0;
		virtual void SetComponentTransform( const std::shared_ptr< PositionComponent >& component, const Vector2& position, float rotation ) = 0;
	};
}