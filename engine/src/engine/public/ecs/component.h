#pragma once

namespace core
{
	class Entity;
	class Component;
	class GameInstance;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	struct ComponentInitContext
	{
		std::shared_ptr< Entity > m_owner;
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class Component : public Serializable, public std::enable_shared_from_this< Component >
	{
		ORB_DECLARE_TYPE( Component );
		ORB_SUPER_CLASS( Serializable );

	public:
		virtual ~Component();

		virtual bool Deserialize( const json& data ) override;

		void Init( const ComponentInitContext& initCtx );
		void Uninit();

		virtual void PostInit();

		virtual void GatherComponents( std::vector< std::shared_ptr< Component > >& components );

		std::shared_ptr< Entity > GetEntityOwner() const { return m_owner.lock(); }

		static bool AreEntityOwnersRelated( const Component& c1, const Component& c2 );

	protected:
		virtual void OnInit( const ComponentInitContext& initCtx );
		virtual void OnUninit();

		const GameInstance* GetGameInstance() const;

	private:
		std::string m_name;

		std::weak_ptr< Entity > m_owner;
	};
}