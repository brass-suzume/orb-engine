cmake_minimum_required (VERSION 3.16)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

set( NLOHMANN_JSON_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ext/nlohmann )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

project( orb )

add_subdirectory( ext )
add_subdirectory( engine )