#include "common/counter.h"

#include <limits>
#include <cassert>

namespace lunar
{

Counter::
Counter( ID_Type init )
	: current_id{ init }
{}

auto Counter::
	 next()
  -> ID_Type
{
    assert( 1u + current_id != std::numeric_limits<ID_Type>::max() );
    return ++current_id;
}

auto Counter::
	 current()
  -> ID_Type
{
	return current_id;
}
} // namespace lunar
