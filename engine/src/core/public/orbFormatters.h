#include <format>

namespace core
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	//
	template< class TChar >
	using OrbCustomFormatterBaseClass = std::formatter< std::basic_string< TChar, std::char_traits< TChar >, std::allocator< TChar > >, TChar >;


	////////////////////////////////////////////////////////////////////////////////////////////////
	// Base formatter for orb types
	// It scans a format string until it finds a delimiter '@'. Then the substring from begin to '@' delimiter (inclusive) is passed to handler functor. The remaining part of the format string is passed further to std string formatter.
	// In case '@' delimiter is not found, the entire format string is sent to std string formatter.
	template< class TChar >
	struct OrbCustomFormatter : OrbCustomFormatterBaseClass< TChar >
	{
		using TBaseClass = OrbCustomFormatterBaseClass< TChar >;

		static constexpr const char c_orbCustomFormatterDelimiter = '@';

		template< class TParseContext, class THandler >
		TParseContext::iterator parse( TParseContext& pc, THandler&& handler );
	};
}

#include "orbFormatters.hpp"