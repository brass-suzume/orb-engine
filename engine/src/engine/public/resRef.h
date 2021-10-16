#pragma once

namespace core
{
	class ResRef : public Serializable
	{
		ORB_DECLARE_TYPE( ResRef );
		ORB_SUPER_CLASS( Serializable );

	public:
		ResRef();
		ResRef( const std::string& resPath );

		virtual bool Deserialize( const json& data ) override;

		const std::string& GetResPath() const { return m_resPath; }

	private:
		std::string m_resPath;
	};
}