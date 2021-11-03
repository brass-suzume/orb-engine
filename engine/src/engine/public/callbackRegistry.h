#pragma once

#include <unordered_map>

namespace core
{
	template< class TFunctor >
	class CallbackRegistry
	{

	public:
		using TCallbackFunctor = std::function< TFunctor >;

		class ID
		{

		public:
			ID() : m_id{ 0 } {}

			explicit operator bool() const { return m_id != INVALID.m_id; }
			bool operator<( const ID& other ) const { return m_id < other.m_id; }

		private:
			ID( size_t id ) : m_id{ id } {}

			friend class CallbackRegistry;
			size_t m_id;
		};

		static const ID INVALID;

	public:
		virtual ~CallbackRegistry()
		{
		}

		ID RegisterCallback( TCallbackFunctor&& functor )
		{
			ID newId{ ++m_nextId };
			m_callbacks[ newId.m_id ] = std::forward< TCallbackFunctor >( functor );
			return newId;
		}

		bool UnregisterCallback( ID& id )
		{
			const bool erased = m_callbacks.erase( id.m_id ) == 1;
			id.m_id = INVALID.m_id;
			return erased;
		}

		template< class... TCallbackArgs >
		void Notify( TCallbackArgs&&... args )
		{
			for( auto it : m_callbacks )
			{
				it.second( std::forward< TCallbackArgs >( args )... );
			}
		}
	private:
		size_t m_nextId = 0;
		std::unordered_map< size_t, TCallbackFunctor > m_callbacks;
	};

	template< class TFunctor >
	typename const CallbackRegistry< TFunctor >::ID CallbackRegistry< TFunctor >::INVALID{ 0 };

}