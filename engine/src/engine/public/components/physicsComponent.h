#pragma once

#include "engine/public/ecs/component.h"

namespace core
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class PhysicsComponent : public Component
	{
		ORB_DECLARE_TYPE( PhysicsComponent );
		ORB_SUPER_CLASS( Component );

	public:
		virtual bool Deserialize( const json& data ) override;

		virtual void PostInit() override;

		void Integrate( float dt );

		const Vector2& GetAcceleration() const { return m_acceleration; }
		const Vector2& GetVelocity() const { return m_velocity; }
		void ForceVelocity( const Vector2& v ) { m_velocity = v; }

		float GetAngularAcceleration() const { return m_angularAcceleration; }
		float GetAngularVelocity() const { return m_angularVelocity; }
		void ForceAngularVelocity( float v ) { m_angularVelocity = v; }

		void AddForce( const Vector2& force );
		void AddTorque( float torque );

		const BoundingSphere& GetBoundingSphere() const { return m_boundingSphere; }

		CollisionCbReg::ID RegisterCollisionCallback( CollisionCbReg::TCallbackFunctor&& callback );
		void UnregisterCollisionCallback( CollisionCbReg::ID& callbackId );

		void OnCollision( const PhysicsComponent& other );

	protected:
		virtual void OnInit( const ComponentInitContext& initCtx ) override;
		virtual void OnUninit() override;

	private:
		float m_mass = 1.0f;
		float m_maxVelocity = 0.0f;
		float m_maxAngularVelocity = 0.0f;

		Vector2 m_acceleration;
		Vector2 m_velocity;

		float m_angularAcceleration = 0.0f;
		float m_angularVelocity = 0.0f;

		BoundingSphere m_boundingSphere{ Vector3::Zero, 0.0f };

		CollisionCbReg m_callbackRegistry;
	};
}