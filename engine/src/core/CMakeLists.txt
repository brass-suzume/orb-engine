project( orbCore )

add_library( ${PROJECT_NAME} ${ORB_CORE_SRC} )
find_package( DirectXTK CONFIG REQUIRED )

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
								Microsoft::DirectXTK )

target_include_directories( ${PROJECT_NAME}
								PUBLIC							
									${DirectXTK_INCLUDE_DIR}
								PRIVATE
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private
									${NLOHMANN_JSON_DIRECTORY} )

target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									<cstdio>
									<string>
									${NLOHMANN_JSON_DIRECTORY}/json.hpp
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