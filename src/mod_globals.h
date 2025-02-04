#ifndef MOD_GLOBALS
#define MOD_GLOBALS

#include "modding.h"
#include "global.h"

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));
RECOMP_IMPORT("*", void* recomp_alloc(size_t size));
RECOMP_IMPORT("*", void recomp_free(void* ptr));

#endif