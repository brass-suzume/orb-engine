#pragma once

namespace core
{
	using CreateFunctor = std::function< std::shared_ptr< Serializable >() >;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	class DataCenter
	{

	public:
		static DataCenter& GetInstance();

		std::string GetFullResourcePath( const std::string& resPath );

		template< class TSerializable >
		void RegisterSerializableType()
		{
			RegisterSerializableType( TSerializable::GetTypeName(), []() { return std::make_shared< TSerializable >(); } );
		}

		template< class TSerializable >
		void RegisterAbstractSerializableType()
		{
			RegisterSerializableType( TSerializable::GetTypeName(), []() { ORB_FAIL( "Trying to instantiate abstarct type" ); return nullptr; } );
		}

		std::shared_ptr< Serializable > CreateSerializableFromResource( const char* resPath );

		template< class TSerializable >
		std::shared_ptr< TSerializable > CreateSerializableFromResource( const char* resPath )
		{
			auto ret = std::dynamic_pointer_cast< TSerializable >( CreateSerializableFromResource( resPath, TSerializable::GetTypeName() ) );
			ORB_ASSERT( ret, "Runtime type mismatch" );
			return ret;
		}

		template< class TSerializable >
		std::shared_ptr< TSerializable > CreateSerializableFromResource( const std::string& resPath )
		{
			auto ret = std::dynamic_pointer_cast< TSerializable >( CreateSerializableFromResource( resPath, TSerializable::GetTypeName() ) );
			ORB_ASSERT( ret, "Runtime type mismatch" );
			return ret;
		}

		template< class TSerializable >
		std::shared_ptr< TSerializable > CreateSerializableFromJson( const json& dataObject )
		{
			auto ret = std::dynamic_pointer_cast< TSerializable >( CreateSerializableFromJson( dataObject ) );
			ORB_ASSERT( ret, "Runtime type mismatch" );
			return ret;
		}

		template< class TSerializable >
		std::shared_ptr< TSerializable > CreateSerializableFromTypeName( const std::string& typeName )
		{
			auto ret = std::dynamic_pointer_cast< TSerializable >( CreateSerializableFromTypeName( typeName ) );
			ORB_ASSERT( ret, "Runtime type mismatch" );
			return ret;
		}

	private:
		DataCenter();

		void RegisterSerializableType( const std::string& typeName, CreateFunctor&& createFunctor );

		std::shared_ptr< Serializable > CreateSerializableFromResource( const std::string& resPath, const char* expectedTypeName );
		std::shared_ptr< Serializable > CreateSerializableFromJson( const json& dataObject );
		std::shared_ptr< Serializable > CreateSerializableFromTypeName( const std::string& typeName );

	private:
		std::unordered_map< std::string, CreateFunctor > m_serializableFactory;
	};
}