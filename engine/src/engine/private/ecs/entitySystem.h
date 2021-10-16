#pragma once

#include "entitySystemInterface.h"

namespace core
{
	class EntitySystem : public IEntitySystem
	{
		
	private:
		struct CreateEntry
		{
			EntityID m_entityId = c_invalidEntityID;
			std::string m_resPath;
			Vector2 m_spawnPosition;
			float m_spawnRotation = 0.0f;
			EntityCreatedCallback m_callback;
			std::weak_ptr< Entity > m_owner;
		};

		struct RegisterEntry
		{
			std::weak_ptr< EntityRef > m_ref;
			EntityRefRegisteredCallback m_callback;
		};

		struct UpdateTransformEntry
		{
			struct Data
			{
				Vector2 m_position;
				float m_rotation;
			};

			struct ComponentData
			{
				std::weak_ptr< PositionComponent > m_component;
				Data m_data;
			};

			bool m_entityUpdate = false;
			Data m_entityData;
			std::vector< ComponentData > m_components;
		};

	public:
		virtual EntityID CreateEntity( const std::string& resPath, const Vector2& position, float rotation, EntityCreatedCallback&& callback, const std::shared_ptr< Entity >& owner ) override;
		virtual void DestroyEntity( EntityID entityId ) override;

		virtual void RegisterEntityRef( const std::shared_ptr< EntityRef >& ref, EntityRefRegisteredCallback&& callback ) override;
		virtual void UnregisterEntityRef( const std::shared_ptr< EntityRef >& ref ) override;

		virtual void SetEntityTransform( EntityID entityId, const Vector2& position, float rotation ) override;
		virtual void SetComponentTransform( const std::shared_ptr< PositionComponent >& component, const Vector2& position, float rotation ) override;

		virtual void OnLevelLoad( const std::shared_ptr< Level >& level ) override;
		virtual void OnLevelUnload( const std::shared_ptr< Level >& level ) override;

	protected:
		virtual void OnInit( const GameSystemInitContext& initCtx ) override;
		virtual void OnUninit() override;

	private:
		void Tick();
		void Tick_ProcessCreations();
		void Tick_ProcessRegisters();
		void Tick_ProcessTransformUpdates();

		void AddEntity( const CreateEntry & entry );
		void AddEntityRef( const std::shared_ptr< EntityRef >& entRef, const EntityRefRegisteredCallback& callback );
		bool RemoveEntityRef( const std::shared_ptr< EntityRef >& entRef );

		std::shared_ptr< Entity > AddEntity( const std::string& resPath, const Vector2& position, float rotation );
		std::shared_ptr< Entity > AddEntity( EntityID entityId, const std::string& resPath, const Vector2& position, float rotation, const std::weak_ptr< Entity >& owner );

	private:
		std::vector< CreateEntry > m_entitiesToCreate;
		std::vector< RegisterEntry > m_refsToProcess;

		EntityID m_entityIdGen = c_invalidEntityID;
		std::unordered_map< EntityID, std::shared_ptr< Entity > > m_entities;
		std::unordered_map< EntityID, std::weak_ptr< EntityRef > > m_registeredRefs;

		std::unordered_map< EntityID, UpdateTransformEntry > m_transformsToUpdate;
	};
}