#pragma once

namespace lunar
{

template< typename... Fs >
struct Overload_Set : Fs...
{
	using Fs::operator()...;
};

template< typename... Fs >
Overload_Set( Fs... ) -> Overload_Set< Fs... >;

} // namespace lunar
