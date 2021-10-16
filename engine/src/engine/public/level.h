#pragma once

namespace core
{
	class EntityRef;
	class GameMode;
	class GameInstance;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	void to_json( json& j, const LevelSpawnPoint& sp );
	void from_json( const json& j, LevelSpawnPoint& sp );


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Level : public Serializable
	{
		ORB_SUPER_CLASS( Serializable );
		ORB_DECLARE_TYPE( Level );

	public:
		virtual bool Deserialize( const json& data ) override;

		void Init( GameInstance* gameInstance );
		void Uninit();

		virtual void Tick( float dt );

		const std::string& GetLevelName() const { return m_levelName; }
		const std::string& GetUserInterfacePath() const { return m_userInterfacePath; }
		const std::vector< std::shared_ptr< EntityRef > >& GetLevelEntities() const { return m_levelEntities; }
		const std::shared_ptr< GameMode >& GetGameMode() const { return m_gameMode; }
		const std::string& GetSkybox() const { return m_skybox; }
		const Vector2& GetMapBounds() const { return m_mapBounds; }
		const std::vector< LevelSpawnPoint >& GetSpawnPoints() const { return m_spawnPoints; }

	protected:
		virtual void OnInit( GameInstance* gameInstance );
		virtual void OnUninit();

	private:
		std::string m_levelName;
		std::string m_userInterfacePath;
		std::shared_ptr< GameMode > m_gameMode;
		std::vector< std::shared_ptr< EntityRef > > m_levelEntities;
		std::string m_skybox;
		Vector2 m_mapBounds;
		std::vector< LevelSpawnPoint > m_spawnPoints;
	};
}