#pragma once

namespace core
{
	class EntityRef;
	class GameInstance;

	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class GameMode : public Serializable
	{
		ORB_DECLARE_TYPE( GameMode );
		ORB_SUPER_CLASS( Serializable );

	public:
		virtual bool Deserialize( const json& data ) override;

		virtual bool IsFinished() const { return false; }

		void Init( GameInstance* gameInstance );
		void Uninit();
		virtual void Tick( float dt );

		bool ShouldSpawnPlayer() const { return m_spawnPlayer; }
		const std::string& GetPlayerEntityResPath() const { return m_playerEntityResPath; }

	protected:
		GameInstance* GetGameInstance() const { return m_gameInstance; }

		virtual void OnInit();
		virtual void OnUninit();

	private:
		bool m_spawnPlayer = true;
		std::string m_playerEntityResPath;

		GameInstance* m_gameInstance = nullptr;

	};
}