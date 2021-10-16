#include "orbCommon.h"

#include <string>
#include <codecvt>
#include <locale>


namespace core
{
	namespace
	{
		using convert_t = std::codecvt_utf8< wchar_t >;
		std::wstring_convert< convert_t, wchar_t > s_stringConverter;
	}

	std::wstring ToWString( const std::string& str )
	{
		return s_stringConverter.from_bytes( str );
	}
}