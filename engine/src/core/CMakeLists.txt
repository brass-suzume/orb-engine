project( orbCore )

add_library( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
					PRIVATE
						private/orbAssert.cpp
						private/orbCommon.cpp

					PUBLIC
						public/orbAssert.h
						public/orbCommon.h
						public/macroHelpers.h )
						
target_include_directories( ${PROJECT_NAME}
								PRIVATE
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private )

target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									<cstdio>
									<string>								
									<memory>
									public/macroHelpers.h
									public/orbAssert.h
									public/orbCommon.h )