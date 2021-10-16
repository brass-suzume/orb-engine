#include "component.h"

#include "entity.h"


namespace core
{
	Component::~Component()
	{
		ORB_ASSERT( m_owner.expired(), "Component still initialized" );
	}

	bool Component::Deserialize( const json& data )
	{
		ORB_DESERIALIZE_PROPERTY( m_name );
		return Super::Deserialize( data );
	}

	void Component::Init( const ComponentInitContext& initCtx )
	{
		ORB_ASSERT( m_owner.expired(), "Owner already assigned" );
		m_owner = initCtx.m_owner;

		OnInit( initCtx );
	}

	void Component::Uninit()
	{
		OnUninit();

		m_owner.reset();
	}

	void Component::PostInit()
	{
	}

	void Component::GatherComponents( std::vector< std::shared_ptr< Component > >& components )
	{
		components.push_back( shared_from_this() );
	}

	bool Component::AreEntityOwnersRelated( const Component& c1, const Component& c2 )
	{
		auto owner1 = c1.GetEntityOwner();
		auto owner2 = c2.GetEntityOwner();
		auto owner1Owner = owner1->GetOwner();
		auto owner2Owner = owner2->GetOwner();

		return owner1 == owner2 ||
			owner1->GetOwner() == owner2 ||
			owner1 == owner2->GetOwner() ||
			( owner1Owner && owner2Owner && owner1->GetOwner() == owner2->GetOwner() );
	}

	void Component::OnInit( const ComponentInitContext& initCtx )
	{
	}

	void Component::OnUninit()
	{
	}

	const GameInstance* Component::GetGameInstance() const
	{
		return GetEntityOwner()->GetGameInstance();
	}
}