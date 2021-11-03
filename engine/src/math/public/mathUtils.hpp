namespace math
{
	bool IsZero( float val, float eps )
	{
		return abs( val ) <= eps;
	}

	constexpr float RadToDeg( float rad )
	{
		return ( rad * 180.0f ) / NUM_PI;
	}

	constexpr float DegToRad( float deg )
	{
		return ( deg * NUM_PI ) / 180.0f;
	}
}