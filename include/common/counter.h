#pragma once

#include "id.h"

namespace lunar
{

// A counter that generates monotonically increasing IDs
class Counter
{
public:

	Counter( ID_Type init = 0u );

	auto next()
	  -> ID_Type;

	auto current()
	  -> ID_Type;

private:

	ID_Type current_id{};
};

} // namespace lunar
