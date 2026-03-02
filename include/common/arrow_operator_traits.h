#pragma once

template< typename T >
concept Has_Arrow_Operator = requires( T t )
{
	t.operator->();
};
