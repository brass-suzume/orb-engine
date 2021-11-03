#pragma once

namespace math
{
	// yaw (-pi; pi), pitch (-pi/2, pi/2), roll (-pi; pi)
	class EulerAngles
	{

	public:
		EulerAngles();
		EulerAngles( float yaw, float pitch, float roll );

		float GetYaw() const;
		float GetPitch() const;
		float GetRoll() const;

		void SetYaw( float y );
		void SetPitch( float p );
		void SetRoll( float r );

	private:
		void Normalize();

	private:
		float m_yaw = 0.0f;
		float m_pitch = 0.0f;
		float m_roll = 0.0f;
	};
}