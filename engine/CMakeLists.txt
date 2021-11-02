
set( ORB_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( ORB_SRC_ROOT_DIR ${ORB_ROOT_DIR}/src )
set( ORB_DATA_ROOT_DIR ${ORB_ROOT_DIR}/data )

find_package( DirectXTK CONFIG REQUIRED )
find_package( DirectXMath CONFIG REQUIRED )
find_package( nlohmann_json CONFIG REQUIRED )

add_subdirectory( src/core )
add_subdirectory( src/math )
add_subdirectory( src/engine )
add_subdirectory( src/prototype )