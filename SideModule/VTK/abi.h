#ifndef ABI_H
#define ABI_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define ABI_FUNC(func) void func

#endif
