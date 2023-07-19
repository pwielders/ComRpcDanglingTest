#pragma once

#ifndef MODULE_NAME
#define MODULE_NAME CustomInterfaces
#endif

#include <core/core.h>
#include <messaging/messaging.h>
#include <com/com.h>

// All identifiers to identify an interface are allocated in this same directory
// in the file calls Ids.h, please extend it with your requried interface number
// if you are creating a new interface.
#include "Ids.h"

#if defined(__WINDOWS__) && defined(COMPILER_DEFAULT_GENERATION)
#undef EXTERNAL
#define EXTERNAL EXTERNAL_EXPORT
#endif