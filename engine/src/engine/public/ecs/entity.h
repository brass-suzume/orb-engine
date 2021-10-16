#pragma once

namespace core
{
	class Component;
	class PositionComponent;
	class GameInstance;
	class Entity;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct EntityInitContext
	{
		Vector2 m_initialPosition;
		float m_initialRotation = 0.0f;
		EntityID m_entityId = c_invalidEntityID;
		const GameInstance* m_gameInstance = nullptr;
		std::weak_ptr< Entity > m_owner;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Entity : public Serializable, public std::enable_shared_from_this< Entity >
	{
		ORB_DECLARE_TYPE( Entity );
		ORB_SUPER_CLASS( Serializable );

	private:
		friend class EntitySystem;

	public:
		virtual ~Entity();

		void Init( const EntityInitContext& initCtx );
		void Uninit();

		EntityID GetEntityID() const { return m_entityId; }
		std::shared_ptr< Entity > GetOwner() const { return m_owner.lock(); }

		Vector2 GetPosition() const { return m_position; }
		float GetRotation() const { return m_rotation; }
		Matrix GetWorldTransform() const;
		Vector2 GetForward() const;
		Vector2 GetRight() const;

		const GameInstance* GetGameInstance() const { return m_gameInstance; }

		virtual bool Deserialize( const json& data ) override;

		template< class TComponent >
		std::shared_ptr< TComponent > FindFirstComponentOfType() const
		{
			for( const std::shared_ptr< Component >& c : m_allComponents )
			{
				if( auto casted = std::dynamic_pointer_cast< TComponent >( c ) )
				{
					return casted;
				}
			}

			return nullptr;
		}

		template< class TComponent, class TFunctor >
		void ForEachComponentOfType( const TFunctor& functor ) const
		{
			for( const std::shared_ptr< Component >& c : m_allComponents )
			{
				if( auto casted = std::dynamic_pointer_cast< TComponent >( c ) )
				{
					functor( casted );
				}
			}
		}

	protected:
		virtual void OnInit( const EntityInitContext& initCtx );
		virtual void OnUninit();

	private:
		void Internal_RefreshHierarchy();
		void Internal_UpdateTransformHierarchy( const Vector2& position, float rotation );

	private:
		EntityID m_entityId = c_invalidEntityID;
		std::weak_ptr< Entity > m_owner;

		const GameInstance* m_gameInstance = nullptr;

		Vector2 m_position;
		float m_rotation = 0.0f;

		std::vector< std::shared_ptr< Component > > m_components;
		std::vector< std::shared_ptr< PositionComponent > > m_positionComponents;
		std::vector< std::shared_ptr< Component > > m_allComponents;
	};
}