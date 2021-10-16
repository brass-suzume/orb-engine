#include "dataCenter.h"

#include <fstream>


namespace core
{
	namespace
	{
		const char* c_dataDirectory = "data/";
		const std::string c_dataFileExtension = ".json";


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		bool HasExtension( const std::string& str )
		{
			return str.find( '.' ) != std::string::npos;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	DataCenter& DataCenter::GetInstance()
	{
		static DataCenter s_instance;
		return s_instance;
	}

	std::string DataCenter::GetFullResourcePath( const std::string& resPath )
	{
		if( !HasExtension( resPath ) )
		{
			return c_dataDirectory + resPath + c_dataFileExtension;
		}
		else
		{
			return c_dataDirectory + resPath;
		}
	}

	std::shared_ptr< Serializable > DataCenter::CreateSerializableFromResource( const char* resPath )
	{
		return CreateSerializableFromResource( resPath, nullptr );
	}

	DataCenter::DataCenter() = default;

	void DataCenter::RegisterSerializableType( const std::string& typeName, CreateFunctor&& createFunctor )
	{
		ORB_ASSERT( m_serializableFactory.count( typeName ) == 0, "Serializable class already registered" );
		m_serializableFactory[ typeName ] = std::move( createFunctor );
	}

	std::shared_ptr< Serializable > DataCenter::CreateSerializableFromResource( const std::string& resPath, const char* expectedTypeName )
	{
		std::string properResPath = GetFullResourcePath( resPath );

		std::ifstream dataFile{ properResPath };
		ORB_ASSERT( dataFile.is_open(), "Failed to open a file" );

		json dataObject;
		dataFile >> dataObject;

		return CreateSerializableFromJson( dataObject );
	}

	std::shared_ptr< Serializable > DataCenter::CreateSerializableFromJson( const json& dataObject )
	{
		ORB_ASSERT( dataObject.contains( "typeName" ), "Json object doesn't have typeName field specified" );

		const std::string& typeName = dataObject[ "typeName" ];
		ORB_ASSERT( m_serializableFactory.count( typeName ) == 1, "Missing typeName" );

		std::shared_ptr< Serializable > newObject = m_serializableFactory[ typeName ]();
		if( newObject->Deserialize( dataObject ) )
		{
			return newObject;
		}
		else
		{
			ORB_ASSERT( false, "Failed to deserialize object" );
			return nullptr;
		}
	}

	std::shared_ptr< Serializable > DataCenter::CreateSerializableFromTypeName( const std::string& typeName )
	{
		return m_serializableFactory[ typeName ]();
	}
}