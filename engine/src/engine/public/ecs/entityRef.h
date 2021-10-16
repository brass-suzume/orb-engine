#pragma once

#include "engine/public/resRef.h"

namespace core
{
	class Entity;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class EntityRef : public ResRef
	{
		ORB_DECLARE_TYPE( EntityRef );
		ORB_SUPER_CLASS( ResRef );

	private:
		friend class EntitySystem;

	public:
		EntityRef();
		EntityRef( const std::string& resPath, const Vector2& position, float rotation );

		virtual bool Deserialize( const json& data ) override;

		const std::shared_ptr< Entity >& GetEntity() const { return m_entity; }
		const Vector2& GetPosition() const { return m_position; }
		float GetRotation() const { return m_rotation; }

	private:
		void SetEntity( std::shared_ptr< Entity >&& entity ) { m_entity = std::move( entity ); }

	private:
		Vector2 m_position;
		float m_rotation = 0.0f;

		std::shared_ptr< Entity > m_entity;
	};
}