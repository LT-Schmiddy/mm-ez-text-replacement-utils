#ifndef __MOD_GLOBALS__
#define __MOD_GLOBALS__

#include "modding.h"
#include "global.h"

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));
RECOMP_IMPORT("*", void* recomp_alloc(size_t size));
RECOMP_IMPORT("*", void recomp_free(void* ptr));

RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char* key));
RECOMP_IMPORT("*", double recomp_get_config_double(const char* key));
RECOMP_IMPORT("*", char* recomp_get_config_string(const char* key));
RECOMP_IMPORT("*", void recomp_free_config_string(char* str));

#define TEXT_DUMPING recomp_get_config_u32("text_dumping")
#define TEXT_DUMPING_FORMAT recomp_get_config_u32("text_dumping_byte_format")

#include "recomp_logging.h"

#endif