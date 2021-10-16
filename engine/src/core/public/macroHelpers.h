#pragma once

#define ORB_EXPAND( ARG ) ARG

#define ORB_GET_MACRO_2( _1, _2, NAME, ... ) NAME

#define ORB_STRINGIFY_EX( expr ) #expr
#define ORB_STRINGIFY( expr ) ORB_STRINGIFY_EX( expr )
#define ORB_CONCATENATE_STR_EX( str1, str2 ) str1##str2
#define ORB_CONCATENATE_STR( str1, str2 ) ORB_CONCATENATE_STR_EX( str1, str2 )