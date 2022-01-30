
namespace core
{
	template< class TChar >
	template< class TParseContext, class THandler >
	TParseContext::iterator OrbCustomFormatter< TChar >::parse( TParseContext& pc, THandler&& handler )
	{
		auto it = pc.begin();

		while( it != pc.end() )
		{
			if( *it++ == c_orbCustomFormatterDelimiter )
			{
				handler( pc.begin(), it );
				pc.advance_to( it + 1 );
				return TBaseClass::parse( pc );
			}
		}

		return TBaseClass::parse( pc );
	}
}