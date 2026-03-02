#pragma once

#include <cassert>
#include <functional>
#include <type_traits>

#include "ref.h"
#include "variant.h"
#include "arrow_operator_traits.h"
#include "meta/specializes_template.h"

namespace lunar
{

struct Nothing
{
};

namespace maybe
{
inline Nothing nothing;
}

template< typename T >
class Maybe;

// Function has call operator that returns some Maybe< T >
template< typename T, typename Function >
concept Returns_Maybe = requires( Function&& f, T&& arg )
{
	{ f( arg ) } -> meta::specialization_of_template< Maybe >;
};


template< typename T >
class Maybe
{
public:

	// empty by default
	Maybe();

	Maybe( T value );

	Maybe( Nothing empty_value );

	Maybe( Maybe const& other );

	Maybe( Maybe&& other );

	auto operator=( Maybe const& other )
	  -> Maybe&;

	auto operator=( Maybe&& other )
	  -> Maybe&;

	auto operator=( T value )
	  -> Maybe&;

	auto operator=( Nothing empty_value )
	  -> Maybe&;

	// returns true if we have a value
	operator bool() const;

	// get value if it exists
	auto operator*() const
	  -> decltype( auto );

	// get value if it exists
	auto operator*()
	  -> decltype( auto );

	// member access into the value
	auto operator->() const
	  -> decltype( auto );

	// member access into the value
	auto operator->()
	  -> decltype( auto );

	// returns true if we have a value
	auto has_value() const
	  -> bool;

	// get value if it exists
	auto get_value() const
	  -> decltype( auto );

	// get value if it exists
	auto get_value()
	  -> decltype( auto );

	// get value if it exists, or the given default
	auto value_or( T default_value ) const
	  -> T;

	// get value if it exists, or the given default
	auto value_or( T default_value )
	  -> T;

	template< typename F, typename Result_T = std::invoke_result_t< F, T > >
	using map_result =
		std::conditional_t<
			std::is_same_v< Result_T
						  , void
						  >
			, void
			, Maybe< Result_T >
		>;

	// apply the given function
	// Function must be T -> U
	// returns Maybe< U >
	template< typename Function, typename Result_T = map_result< Function > >
	auto map( Function&& function ) const
	  -> Result_T;

	// apply the given function
	// Function must be T -> Maybe< U >
	// returns Maybe< U >
	template< typename Function >
		requires Returns_Maybe< T, Function >
	auto bind( Function&& function ) const
	  -> decltype( function( std::declval< T >() ) );

private:

	Variant< T, Nothing > _value;
};

template< typename T >
Maybe( T t ) -> Maybe< T >;


template< typename T >
using Maybe_Ref = Maybe< Ref< T > >;


template< typename T >
Maybe< T >::
Maybe()
	: _value( maybe::nothing )
{
}

template< typename T >
Maybe< T >::
Maybe( T value )
	: _value( std::move( value ) )
{
}

template< typename T >
Maybe< T >::
Maybe( Nothing empty_value )
	: _value( empty_value )
{
}

template< typename T >
Maybe< T >::
Maybe( Maybe const& other )
	= default;

template< typename T >
Maybe< T >::
Maybe( Maybe&& other )
	: _value( std::move( other._value ) )
{
	other._value = maybe::nothing;
}

template< typename T >
auto Maybe< T >::
	 operator=( Maybe const& other )
  -> Maybe&
   = default;

template< typename T >
auto Maybe< T >::
	 operator=( Maybe&& other )
  -> Maybe&
{
	_value = std::move( other._value );
	other._value = maybe::nothing;
	return *this;
}

template< typename T >
auto Maybe< T >::
	 operator=( T value )
  -> Maybe&
{
	_value = std::move( value );
	return *this;
}

template< typename T >
auto Maybe< T >::
	 operator=( Nothing empty_value )
  -> Maybe&
{
	_value = empty_value;
	return *this;
}

template< typename T >
Maybe< T >::
operator bool() const
{
	return has_value();
}

template< typename T >
auto Maybe< T >::
	 operator* () const
  -> decltype( auto )
{
	return get_value();
}

template< typename T >
auto Maybe< T >::
	 operator* ()
  -> decltype( auto )
{
	return get_value();
}


template< typename T >
auto Maybe< T >::
	 operator->() const
  -> decltype( auto )
{
	if constexpr( Has_Arrow_Operator< T > || std::is_pointer_v< T > )
		return static_cast< T >( get_value() );
	else
		return &get_value();
}

template< typename T >
auto Maybe< T >::
	 operator->()
  -> decltype( auto )
{
	if constexpr( Has_Arrow_Operator< T > || std::is_pointer_v< T > )
		return static_cast< T >( get_value() );
	else
		return &get_value();
}
template< typename T >
auto Maybe< T >::
	 has_value() const
  -> bool
{
	return !holds_alternative< Nothing >( _value );
}

template< typename T >
auto Maybe< T >::
	 get_value() const
  -> decltype( auto )
{
	assert( has_value() );
	return get< 0 >( _value );
}

template< typename T >
auto Maybe< T >::
	 get_value()
  -> decltype( auto )
{
	assert( has_value() );
	return get< 0 >( _value );
}

template< typename T >
auto Maybe< T >::
	 value_or( T default_value ) const
  -> T
{
	if( has_value() )
		return get_value();
	else
		return std::move( default_value );
}

template< typename T >
auto Maybe< T >::
	 value_or( T default_value )
  -> T
{
	if( has_value() )
		return get_value();
	else
		return std::move( default_value );
}

template< typename T >
template< typename Function, typename Result_T >
auto Maybe< T >::
	 map( Function&& function ) const
  -> Result_T
{
	static constexpr
	bool function_has_return_type =
		! std::is_same_v< Result_T, void >;

	if constexpr( function_has_return_type )
	{
		if( has_value() )
			return { function( get_value() ) };
		else
			return { maybe::nothing };
	}
	else
	{
		function( get_value() );
	}
}

template< typename T >
template< typename Function >
	requires Returns_Maybe< T, Function >
auto Maybe< T >::
	 bind( Function&& function ) const
  -> decltype( function( std::declval< T >() ) )
{
	if( has_value() )
		return function( get_value() );
	else
		return maybe::nothing;
}

}
