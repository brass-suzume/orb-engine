project( orbPrototype )

add_executable( ${PROJECT_NAME} WIN32 )

target_sources( ${PROJECT_NAME} 
                    PRIVATE
                        $<TARGET_OBJECTS:orbEngine>
                        private/main.cpp  "private/protoRuntimeStage.cpp" "private/protoRuntimeStage.h")

target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC orbEngine )
target_include_directories( ${PROJECT_NAME} PRIVATE ${ORB_SRC_ROOT_DIR} )
target_precompile_headers( ${PROJECT_NAME} REUSE_FROM orbEngine )

ADD_CUSTOM_TARGET( link_data ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${ORB_DATA_ROOT_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data )
