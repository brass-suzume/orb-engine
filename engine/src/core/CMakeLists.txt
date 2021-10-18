project( orbCore )

find_package( DirectXTK CONFIG REQUIRED )
find_package( DirectXMath CONFIG REQUIRED )
find_package( nlohmann_json CONFIG REQUIRED )

add_library( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
					PRIVATE
						private/orbAssert.cpp
						private/serializable.cpp
						private/serializationHelpers.cpp
						private/dataCenter.cpp
						private/orbMath.cpp
						private/orbCommon.cpp

					PUBLIC
						public/orbAssert.h
						public/serializable.h
						public/serializationHelpers.h
						public/dataCenter.h
						public/type.h
						public/orbMath.h
						public/orbForwards.h
						public/orbCommon.h
						public/macroHelpers.h
						public/typeRegistrar.h
						public/callbackRegistry.h )

target_link_libraries( ${PROJECT_NAME}
							PUBLIC
								Microsoft::DirectXTK
								nlohmann_json::nlohmann_json )

target_include_directories( ${PROJECT_NAME}
								PRIVATE
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private )

target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									<cstdio>
									<string>
									<nlohmann/json.hpp>
									public/macroHelpers.h
									public/orbForwards.h
									public/orbAssert.h
									public/orbMath.h
									public/orbCommon.h
									public/type.h
									public/serializable.h
									public/serializationHelpers.h
									public/dataCenter.h
									public/typeRegistrar.h
									public/callbackRegistry.h )