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

private:

	ID_Type next_id{};
};

} // namespace lunar
