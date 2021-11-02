project( orbPrototype )

add_executable( ${PROJECT_NAME} WIN32 )

target_sources( ${PROJECT_NAME} 
                    PRIVATE
                        private/main.cpp
                        private/protoRuntimeStage.cpp
                        private/protoRuntimeStage.h)

target_link_libraries( ${PROJECT_NAME} PRIVATE orbEngine )
target_include_directories( ${PROJECT_NAME} PRIVATE ${ORB_SRC_ROOT_DIR} )
#target_precompile_headers( ${PROJECT_NAME} REUSE_FROM orbEngine )

add_custom_target( link_data ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${ORB_DATA_ROOT_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data )
