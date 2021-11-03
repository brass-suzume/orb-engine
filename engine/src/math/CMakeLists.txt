project( orbMath )

add_library( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME}
					PRIVATE
						private/orbMath.cpp

					PUBLIC
						public/orbMath.h
						public/eulerAngles.h
						public/eulerAngles.hpp 
						public/quaternion.h 
						public/quaternion.hpp
						public/transform.h 
						public/transform.hpp
						public/mathUtils.h 
						public/mathUtils.hpp
						public/mathTypes.h )

target_link_libraries( ${PROJECT_NAME}
							PUBLIC
								Microsoft::DirectXTK
							PRIVATE
								orbCore )

target_include_directories( ${PROJECT_NAME}
								PRIVATE
									${PROJECT_SOURCE_DIR}/public
									${PROJECT_SOURCE_DIR}/private )

target_precompile_headers( ${PROJECT_NAME} 
								PUBLIC
									public/orbMath.h )