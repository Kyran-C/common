#include "common/counter.h"

#include <limits>
#include <cassert>

namespace lunar
{

Counter::
Counter( ID_Type init )
	: next_id{ init }
{}

auto Counter::
	 next()
  -> ID_Type
{
    assert( 1u + next_id != std::numeric_limits<ID_Type>::max() );
    return next_id++;
}

} // namespace lunar
