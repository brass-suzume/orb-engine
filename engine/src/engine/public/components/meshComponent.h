#pragma once

#include "engine/public/ecs/positionComponent.h"


namespace core
{
	class ResRef;
	class Mesh;

	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	class MeshComponent : public PositionComponent
	{
		ORB_DECLARE_TYPE( MeshComponent );
		ORB_SUPER_CLASS( PositionComponent );

	public:
		virtual ~MeshComponent();
		virtual bool Deserialize( const json& data ) override;

		const std::shared_ptr< Mesh >& GetMesh() const { return m_mesh; }
		float GetBoundingSphere() const;

	protected:
		virtual void OnInit( const ComponentInitContext& initCtx ) override;
		virtual void OnUninit() override;
		virtual void OnTransformUpdated() override;

	private:
		std::shared_ptr< ResRef > m_meshRef;
		float m_scale = 1.0f;

		RenderProxyID m_meshProxyId;
		std::shared_ptr< Mesh > m_mesh;
	};
}