
namespace math
{
	EulerAngles::EulerAngles() {}

	EulerAngles::EulerAngles( float yaw, float pitch, float roll )
		: m_yaw{ yaw }
		, m_pitch{ pitch }
		, m_roll{ roll }
	{
		Normalize();
	}

	float EulerAngles::GetYaw() const
	{
		return m_yaw;
	}

	float EulerAngles::GetPitch() const
	{
		return m_pitch;
	}

	float EulerAngles::GetRoll() const
	{
		return m_roll;
	}

	void EulerAngles::SetYaw( float y )
	{
		m_yaw = y;
	}

	void EulerAngles::Normalize()
	{
		m_yaw = fmodf( m_yaw, math::NUM_PI );
		m_pitch = fmodf( m_pitch, math::NUM_PI_DIV_2 );
		m_roll = fmodf( m_roll, math::NUM_PI );
	}
}