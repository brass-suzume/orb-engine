#pragma once

namespace core
{
	class IGameSystem;
	class GameInstance;
	class Level;
	class ITickRegister;
	class IRenderer;

	using SystemID = uint8_t;

	namespace prv
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		SystemID GenerateID();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class GameSystemRegistrar
	{

	private:
		using CreateFunctor = std::function< std::shared_ptr< IGameSystem >() >;

	public:
		template< class TSystem >
		struct Token
		{
			Token() { GetInstance().RegisterSystemType< TSystem >(); }
		};

	public:
		static GameSystemRegistrar& GetInstance();

		template< class TSystem >
		std::shared_ptr< IGameSystem > CreateSystem() const
		{
			return m_factory.at( TSystem::GetStaticID() )();
		}

	private:
		GameSystemRegistrar();

		template< class TSystem >
		void RegisterSystemType()
		{
			ORB_ASSERT( TSystem::GetStaticID() == 0, "System already registered" );

			const SystemID newId = prv::GenerateID();
			TSystem::s_id = newId;
			m_factory[ newId ] = []() { return std::make_shared< TSystem >(); };
		}

	private:
		std::unordered_map< SystemID, CreateFunctor > m_factory;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct GameSystemInitContext
	{
		GameInstance& m_gameInstance;
		ITickRegister& m_tickRegister;
		const char* m_playerObjectTypename = nullptr;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class IGameSystem : private NoCopy, public std::enable_shared_from_this< IGameSystem >
	{

	public:
		virtual ~IGameSystem() {}

		void Init( const GameSystemInitContext& initCtx );
		void Uninit();
		
		virtual SystemID GetID() const = 0;

		virtual void OnLevelLoad( const std::shared_ptr< Level >& level );
		virtual void OnLevelUnload( const std::shared_ptr< Level >& level );

	protected:
		virtual void OnInit( const GameSystemInitContext& initCtx );
		virtual void OnUninit();

		GameInstance* GetGameInstance() const { return m_gameInstance; }

	private:
		GameInstance* m_gameInstance = nullptr;
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	template< class TSystem >
	class TGameSystem : public IGameSystem
	{
		friend class GameSystemRegistrar;

	public:
		static SystemID GetStaticID() { return s_id; }
		virtual SystemID GetID() const override final { return s_id; }

	private:
		static SystemID s_id;
	};

	template< class TSystem >
	SystemID TGameSystem< TSystem >::s_id = 0;
}

#define ORB_REGISTER_SYSTEM( sysType ) namespace core { namespace prv { static core::GameSystemRegistrar::Token< sysType > s_systemToken##sysType; } }
#define ORB_REGISTER_SYSTEM_IN_NS( ns, sysType ) namespace core { namespace prv { static core::GameSystemRegistrar::Token< ns::sysType > s_systemToken##sysType; } }