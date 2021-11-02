#pragma once

using json = nlohmann::json;

namespace core
{
	class Serializable
	{
		ORB_DECLARE_TYPE( Serializable );

	public:
		virtual ~Serializable() = 0 {};

		virtual bool Deserialize( const json& data );
	};
}