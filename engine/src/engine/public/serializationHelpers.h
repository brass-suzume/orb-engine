#pragma once

namespace core
{
	std::string ExtractRealPropertyName( const char* propertyName );
}

#define _INTERNAL_ORB_DESERIALIZE_PROPERTY( propertyName, mandatory )												\
							do																					\
							{																					\
								static std::string realPropertyName = core::ExtractRealPropertyName( #propertyName );	\
								if( data.contains( realPropertyName.c_str() ) )									\
								{																				\
									propertyName = std::forward< decltype( propertyName )>( data[ realPropertyName.c_str() ] );							\
								}																				\
								else if( mandatory )															\
								{																				\
									ORB_ASSERT( false, "Mandatory property is missing" );						\
									return false;																\
								}																				\
							} while( false );

#define _INTERNAL_ORB_DESERIALIZE_OBJECT_PROPERTY( propertyName, mandatory )												\
							do																					\
							{																					\
								static std::string realPropertyName = core::ExtractRealPropertyName( #propertyName );	\
								if( data.contains( realPropertyName.c_str() ) )									\
								{																				\
									propertyName = core::DataCenter::GetInstance().CreateSerializableFromJson< decltype( propertyName )::element_type >( data[ realPropertyName.c_str() ] ); \
								}																				\
								else if( mandatory )															\
								{																				\
									ORB_ASSERT( false, "Mandatory property is missing" );						\
									return false;																\
								}																				\
							} while( false );

#define _INTERNAL_ORB_DESERIALIZE_OBJECT_ARRAY( propertyName )	\
							do																																	\
							{																																	\
								static std::string realPropertyName = core::ExtractRealPropertyName( #propertyName );											\
								if( data.contains( realPropertyName.c_str() ) )																					\
								{																																\
									auto arrayData = data[ realPropertyName.c_str() ];																			\
									propertyName.reserve( arrayData.size() );																					\
									for( auto& element : arrayData )																							\
									{																															\
										ORB_ASSERT( element.is_object(), "Stored element is not an object" );													\
										using ArrayElementType = decltype( propertyName )::value_type;															\
										using PtrElementType = ArrayElementType::element_type;																	\
										propertyName.push_back( core::DataCenter::GetInstance().CreateSerializableFromJson< PtrElementType >( element ) );		\
									}																															\
								}																																\
							} while( false );

#define ORB_DESERIALIZE_PROPERTY( propertyName ) _INTERNAL_ORB_DESERIALIZE_PROPERTY( propertyName, true )
#define ORB_DESERIALIZE_OPTIONAL_PROPERTY( propertyName ) _INTERNAL_ORB_DESERIALIZE_PROPERTY( propertyName, false )

#define ORB_DESERIALIZE_OBJECT_PROPERTY( propertyName ) _INTERNAL_ORB_DESERIALIZE_OBJECT_PROPERTY( propertyName, true )
#define ORB_DESERIALIZE_OPTIONAL_OBJECT_PROPERTY( propertyName ) _INTERNAL_ORB_DESERIALIZE_OBJECT_PROPERTY( propertyName, false )

#define ORB_DESERIALIZE_OBJECT_ARRAY_PROPERTY( propertyName ) _INTERNAL_ORB_DESERIALIZE_OBJECT_ARRAY( propertyName )
