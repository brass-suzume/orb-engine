#pragma once

namespace core
{
	class GameInstance;
	class Entity;

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Controller : public Serializable
	{
		ORB_DECLARE_TYPE( Controller );
		ORB_SUPER_CLASS( Serializable );

	public:
		virtual ~Controller();

		void Init( const GameInstance& gameInstance );
		void Uninit();

		const std::string& GetName() const { return m_name; }
		void SetName( const std::string& name ) { m_name = name; }

		virtual void Update( float dt ) = 0;

		void SetControlledEntity( const std::shared_ptr< Entity >& entity );
		std::shared_ptr< Entity > GetControlledEntity() const { return m_controlledEntity.lock(); }
		const GameInstance* GetGameInstance() const { return m_gameInstance; }

	protected:
		virtual void OnInit();
		virtual void OnUninit();
		virtual void OnControlledEntityChanged();			

	private:
		const GameInstance* m_gameInstance;
		std::weak_ptr< Entity > m_controlledEntity;
		std::string m_name;
	};
}