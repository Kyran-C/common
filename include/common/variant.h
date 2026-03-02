#pragma once

#include <utility>
#include <variant>
// maybe swap to different variant implementation for better performance

#include "forward.h"
#include "overload_set.h"

namespace lunar
{

template< typename... >
struct Variant;

// template< typename T >
// concept 

template < typename... Ts >
struct Variant
{
	template< typename Variant_T = std::variant< Ts... >
			, typename = std::enable_if_t< std::is_default_constructible_v< Variant_T >, void > >
	constexpr
	Variant()
		: v()
	{}

    template< typename Value_T >
	requires( std::same_as< std::remove_cvref_t< Value_T&& >, Ts > || ... )
    constexpr
	Variant( Value_T&& value )
        : v( std::forward< Value_T >( value ) )
    {}

 //    template< typename... Args >
 //    constexpr
	// Variant( Args&&... args )
 //        : v( std::forward< Args >(args)... )
 //    {}

    constexpr
	Variant( Variant const& other )
        : v( other.v )
    {}

    constexpr
	Variant( Variant&& other ) noexcept
		: v( std::move( other.v ) )
    {}

    constexpr
	auto operator=( Variant const& other )
	  -> Variant&
    {
        v = other.v;
        return *this;
    }

    constexpr
	auto operator=( Variant&& other ) noexcept
	  -> Variant&
    {
        v = std::move( other.v );
        return *this;
    }

    constexpr
	auto index() const noexcept
	  -> std::size_t
    {
        return v.index();
    }

	auto visit( this auto&& self, auto&& visitor )
	{
		return std::visit( FWD( visitor ), FWD( self ).v );
	}

	auto visit( this auto&& self, auto&& visitor_1, auto&& visitor_2, auto&&... remaining_visitors )
	{
		return std::visit( Overload_Set{ FWD( visitor_1 ), FWD( visitor_2 ), FWD( remaining_visitors )... }, FWD( self ).v );
	}

    template< typename T >
    friend constexpr
	auto holds_alternative( Variant< Ts... > const& variant ) noexcept
	  -> bool
    {
        return std::holds_alternative< T, Ts... >( variant.v );
    }

    template< std::size_t I >
    friend constexpr
	auto get( Variant< Ts... >& variant )
      -> std::variant_alternative_t< I, std::variant< Ts... > >&
    {
        return std::get< I >( variant.v );
    }

    template< std::size_t I >
    friend constexpr
	auto get( Variant< Ts... >&& variant )
      -> std::variant_alternative_t< I, std::variant< Ts... > >&&
    {
        return std::get< I >( std::move( variant.v ) );
    }

    template< std::size_t I >
    friend constexpr
	auto get( Variant< Ts... > const& variant )
      -> std::variant_alternative_t< I, std::variant< Ts... > > const&
    {
        return std::get< I >( variant.v );
    }

    template< std::size_t I >
    friend constexpr
	auto get( Variant< Ts... > const&& variant )
      -> std::variant_alternative_t< I, std::variant< Ts... > > const&&
    {
        return std::get< I >( std::move( variant.v ) );
    }

    template< typename T >
    friend constexpr
	auto get( Variant< Ts... >& variant )
	  -> T&
    {
        return std::get< T >( variant.v );
    }

    template< typename T >
    friend constexpr
	auto get( Variant< Ts... >&& variant )
	  -> T&&
    {
        return std::get< T >( std::move( variant.v ) );
    }

    template< typename T >
    friend constexpr
	auto get( Variant< Ts... > const& variant )
	  -> T const&
    {
        return std::get< T >( variant.v );
    }

    template< typename T >
    friend constexpr
	auto get( Variant< Ts... > const&& variant )
	  -> T const&&
    {
        return std::get< T >( std::move( variant.v ) );
    }

private:
    std::variant< Ts... > v;
};

// declarations to bring these friend functions into namespace scope for unqualified lookup:

template< typename T, typename... Ts >
constexpr
auto holds_alternative( Variant< Ts... > const& variant ) noexcept
  -> bool;


template< std::size_t I, typename... Ts >
constexpr
auto get( Variant< Ts... >& variant )
  -> std::variant_alternative_t< I, std::variant< Ts... > >&;

template< std::size_t I, typename... Ts >
constexpr
auto get( Variant< Ts... >&& variant )
  -> std::variant_alternative_t< I, std::variant< Ts... > >&&;

template< std::size_t I, typename... Ts >
constexpr
auto get( Variant< Ts... > const& variant )
  -> std::variant_alternative_t< I, std::variant< Ts... > > const&;

template< std::size_t I, typename... Ts >
constexpr
auto get( Variant< Ts... > const&& variant )
  -> std::variant_alternative_t< I, std::variant< Ts... > > const&&;


template< typename T, typename... Ts >
constexpr
auto get( Variant< Ts... >& variant )
  -> T&;

template< typename T, typename... Ts >
constexpr
auto get( Variant< Ts... >&& variant )
  -> T&&;

template< typename T, typename... Ts >
constexpr
auto get( Variant< Ts... > const& variant )
  -> T const&;

template< typename T, typename... Ts >
constexpr
auto get( Variant< Ts... > const&& variant )
  -> T const&&;

}
