#pragma once

#include <functional>
#include <type_traits>

#include "arrow_operator_traits.h"

namespace lunar
{

namespace detail {

template< typename T >
constexpr
T& as_T( T& t ) noexcept
{
	return t;
}

template< typename T >
void as_T( T&& ) = delete;

template< typename U, typename T >
concept Converts_To_T = requires( U u )
{
	as_T<T>( u );
};

template< typename U, typename T >
concept Non_Reference_Wrapper
	= not std::is_same_v<
			std::reference_wrapper< T >,
			std::remove_cvref_t< U >
		>;

template< typename U, typename T >
concept Valid_Param = Non_Reference_Wrapper< U, T > && Converts_To_T< U, T >;

} // namespace detail
 
 
template< typename T >
class Ref : public std::reference_wrapper< T >
{
public:

	template< detail::Valid_Param< T > U >
	constexpr
	Ref( U&& u ) noexcept( noexcept( detail::as_T<T>( std::forward<U>(u) ) ) )
		: std::reference_wrapper< T >( u )
	{}

	Ref( Ref const& ) noexcept
	   = default;
 
    // assignment
	auto operator=( Ref const& x ) noexcept
      -> Ref&
	   = default;
 
    // access
	using std::reference_wrapper<T>::operator T&;
	using std::reference_wrapper<T>::get;

	auto operator*()
	  -> T&
	{
		return get();
	}

	auto operator->() const
	  -> decltype( auto )
	{
		if constexpr( Has_Arrow_Operator< T > || std::is_pointer_v< T > )
			return get();
		else
			return &get();
	}
};

template< typename T >
Ref( T& ) -> Ref< T >;

}
