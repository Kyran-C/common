#pragma once

#include <algorithm>
#include <functional>
#include <ranges>

#include "forward.h"
#include "maybe.h"

namespace lunar
{
	

/// Function wrapper which counts how many times it's been invoked,
/// and passes that as the last argument to the target function.
/// When iterating a list, this gives you the element index along with the element itself.
/// Note that the returned function is not mutable.
static constexpr
auto indexed = []( auto&& func )
{
	return
		[ index = 0u
		, func = FWD( func )
		]
		( auto&&... args ) mutable
		{
			return func( decltype( args )( args )..., index++ );
		};
};

/// Tries to find an element in the container satisfying the given predicate.
/// Returns Maybe_Ref< E >.
template< typename Lens_T = std::identity >
auto find_if( std::ranges::input_range auto&& container, auto&& predicate, Lens_T&& lens = {} )
  -> Maybe_Ref< std::remove_reference_t< decltype( *container.begin() ) > >
{
    if( auto found = std::ranges::find_if( container, FWD( predicate ), FWD( lens ) );
		found != container.end() )
		return { *found };
    else
        return maybe::nothing;
}

auto find( std::ranges::input_range auto&& container, auto&& element, auto&& lens = std::identity{} )
  -> Maybe_Ref< std::remove_reference_t< decltype( *container.begin() ) > >
{
	if( auto found = std::ranges::find( container, FWD( element ), FWD( lens ) );
		found != container.end() )
		return { { *found } };

	return maybe::nothing;
}

constexpr
auto second = []( auto&& value )
  -> decltype( auto )
{
	return FWD( value ).second;
};

constexpr
auto first = []( auto&& value )
  -> decltype( auto )
{
	return FWD( value ).first;
};

}
