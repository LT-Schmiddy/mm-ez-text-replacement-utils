#ifndef __RECOMP_LOGGING_CONFIG__
#define __RECOMP_LOGGING_CONFIG__

#include "mod_globals.h"

#define RECOMP_LOG_CONFIG_HEADER "EZTR LOG"
#define RECOMP_LOG_CONFIG_PRINTF_FUNC recomp_printf

#define RECOMP_LOG_CONFIG_LOG_LEVEL recomp_get_config_u32("recomp_log_level")
#define RECOMP_LOG_CONFIG_USE_COLORS recomp_get_config_u32("recomp_log_colors")

// ANSI Color codes sourced from here:
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
#define RECOMP_LOG_CONFIG_FATAL_COLOR "\e[0;101m" // Red, High Intensity Background
#define RECOMP_LOG_CONFIG_ERROR_COLOR "\e[0;31m" // Red
#define RECOMP_LOG_CONFIG_WARNING_COLOR "\e[0;33m" // Yellow
#define RECOMP_LOG_CONFIG_INFO_COLOR "\e[0;32m" // Green
#define RECOMP_LOG_CONFIG_DEBUG_COLOR "\e[0;36m" // Cyan
#define RECOMP_LOG_CONFIG_VERBOSE_COLOR "\e[0;35m" // Magenta

#endif