#pragma once

#include <config.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define LIBRARY_EXPORT
#else
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))
#include "library_export.h"
#endif

extern "C" {
EMSCRIPTEN_KEEPALIVE  
int LIBRARY_EXPORT DoWork();
}
