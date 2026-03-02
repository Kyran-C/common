#pragma once

// perfect forward the argument
#define FWD( var ) std::forward< decltype( var ) >( var )
