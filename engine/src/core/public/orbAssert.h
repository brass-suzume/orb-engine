#pragma once


#define _INTERNAL_ORB_ASSERT_1( expression ) _ASSERT( expression )
#define _INTERNAL_ORB_ASSERT_2( expression, msg ) _ASSERT_EXPR( expression, L#msg )

#define ORB_ASSERT( ... ) ORB_EXPAND( ORB_GET_MACRO_2( __VA_ARGS__, _INTERNAL_ORB_ASSERT_2, _INTERNAL_ORB_ASSERT_1 )( __VA_ARGS__ ) )
#define ORB_FAIL( msg ) _ASSERT_EXPR( false, L#msg )