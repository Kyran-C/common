#pragma once

#include <iostream>

namespace lunar::debug
{

constexpr
auto print = []( auto&&... args )
  -> auto&
{
	return (std::cout << ... << args);
};


thread_local
constexpr
auto println = []< typename... Args_T >( Args_T&&... args )
  -> auto&
{
	using std::endl;

	return print( std::forward<Args_T>( args )... )
		<< endl;
};

} // namespace lunar::debug
