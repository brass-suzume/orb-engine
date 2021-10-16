#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
namespace core
{
	namespace prv
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		template< class TType >
		struct TypeRegistrar
		{
			TypeRegistrar()
			{
				DataCenter::GetInstance().RegisterSerializableType< TType >();
			}
		};


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		template< class TType >
		struct AbstractTypeRegistrar
		{
			AbstractTypeRegistrar()
			{
				DataCenter::GetInstance().RegisterAbstractSerializableType< TType >();
			}
		};
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define ORB_REGISTER_TYPE( cls ) namespace core{ namespace prv{ static TypeRegistrar< cls > s_typeRegistrar##cls; } }
#define ORB_REGISTER_TYPE_IN_NS( ns, cls ) namespace core{ namespace prv{ static TypeRegistrar< ns::cls > s_typeRegistrar##ns##cls; } }

#define ORB_REGISTER_ABSTRACT_TYPE( cls ) namespace core{ namespace prv{ static AbstractTypeRegistrar< cls > s_typeRegistrar##cls; } }
#define ORB_REGISTER_ABSTRACT_TYPE_IN_NS( ns, cls ) namespace core{ namespace prv{ static AbstractTypeRegistrar< ns::cls > s_typeRegistrar##ns##cls; } }