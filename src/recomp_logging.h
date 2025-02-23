#ifndef __RECOMP_LOGGING__
#define __RECOMP_LOGGING__

// Config Area:
#include "recomp_logging_config.h"
// End of config:
#ifndef __FILE_NAME__
#define __FILE_NAME__ "Unknown Source File"
#endif

typedef enum {
    RECOMP_LOG_NONE = 0,
    RECOMP_LOG_FATAL= 1,
    RECOMP_LOG_ERROR = 2,
    RECOMP_LOG_WARNING = 3,
    RECOMP_LOG_INFO= 4,
    RECOMP_LOG_DEBUG = 5,
    RECOMP_LOG_VERBOSE = 6
} RecompLogLevel;

static const char* _RECOMP_LOG_LEVEL_STRINGS[] = {
    "NONE",
    "FATAL",
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
    "VERBOSE"
};

// ANSI Color codes sourced from here:
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
#define _CRESET "\e[0m"
static const char* _RECOMP_LOG_LEVEL_COLORS[] = {
    _CRESET,
    RECOMP_LOG_CONFIG_FATAL_COLOR, 
    RECOMP_LOG_CONFIG_ERROR_COLOR, 
    RECOMP_LOG_CONFIG_WARNING_COLOR, 
    RECOMP_LOG_CONFIG_INFO_COLOR, 
    RECOMP_LOG_CONFIG_DEBUG_COLOR, 
    RECOMP_LOG_CONFIG_VERBOSE_COLOR, 
};

#define _LOG_PRINTF_CALL(format, ...) RECOMP_LOG_CONFIG_PRINTF_FUNC(format, __VA_ARGS__)
#define _LOG_PRINT_CALL(format, ...) RECOMP_LOG_CONFIG_PRINTF_FUNC(format)

#define IS_LOG_LEVEL(log_level) log_level <= RECOMP_LOG_CONFIG_LOG_LEVEL
#define IF_LOG_LEVEL(log_level) if (IS_LOG_LEVEL(log_level))

#define IS_LOG_FATAL IS_LOG_LEVEL(RECOMP_LOG_FATAL)
#define IS_LOG_ERROR IS_LOG_LEVEL(RECOMP_LOG_ERROR)
#define IS_LOG_WARNING IS_LOG_LEVEL(RECOMP_LOG_WARNING)
#define IS_LOG_INFO IS_LOG_LEVEL(RECOMP_LOG_INFO)
#define IS_LOG_DEBUG IS_LOG_LEVEL(RECOMP_LOG_DEBUG)
#define IS_LOG_VERBOSE IS_LOG_LEVEL(RECOMP_LOG_VERBOSE)

#define IF_LOG_FATAL IF_LOG_LEVEL(RECOMP_LOG_FATAL)
#define IF_LOG_ERROR IF_LOG_LEVEL(RECOMP_LOG_ERROR)
#define IF_LOG_WARNING IF_LOG_LEVEL(RECOMP_LOG_WARNING)
#define IF_LOG_INFO IF_LOG_LEVEL(RECOMP_LOG_INFO)
#define IF_LOG_DEBUG IF_LOG_LEVEL(RECOMP_LOG_DEBUG)
#define IF_LOG_VERBOSE IF_LOG_LEVEL(RECOMP_LOG_VERBOSE)

#define _LOG_HEADER(log_level) \
    _LOG_PRINTF_CALL("%s%s %s (%s at line %u, in %s): ", _RECOMP_LOG_LEVEL_COLORS[log_level], RECOMP_LOG_CONFIG_HEADER, \
        _RECOMP_LOG_LEVEL_STRINGS[log_level], __FILE_NAME__, __LINE__, __func__);

#define _LOG_FOOTER _LOG_PRINTF_CALL("%s\n", _CRESET);

#define LOG_FORMAT(log_level, format, ...) \
    { IF_LOG_LEVEL(log_level) { \
        _LOG_HEADER(log_level) \
        _LOG_PRINTF_CALL(format, __VA_ARGS__); \
        _LOG_FOOTER \
    }};

#define LOG_NO_FORMAT(log_level, format) \
    { IF_LOG_LEVEL(log_level) { \
        _LOG_HEADER(log_level) \
        _LOG_PRINT_CALL(format); \
        _LOG_FOOTER \
    }};

// Logging Macros:
#define LOG_FATAL_F(format, ...) LOG_FORMAT(RECOMP_LOG_FATAL, format, __VA_ARGS__)
#define LOG_ERROR_F(format, ...) LOG_FORMAT(RECOMP_LOG_ERROR, format, __VA_ARGS__)
#define LOG_WARNING_F(format, ...) LOG_FORMAT(RECOMP_LOG_WARNING, format, __VA_ARGS__)
#define LOG_INFO_F(format, ...) LOG_FORMAT(RECOMP_LOG_INFO, format, __VA_ARGS__)
#define LOG_DEBUG_F(format, ...) LOG_FORMAT(RECOMP_LOG_DEBUG, format, __VA_ARGS__)
#define LOG_VERBOSE_F(format, ...) LOG_FORMAT(RECOMP_LOG_VERBOSE, format, __VA_ARGS__)

#define LOG_FATAL(format), LOG_NO_FORMAT(RECOMP_LOG_FATAL, format)
#define LOG_ERROR(format) LOG_NO_FORMAT(RECOMP_LOG_ERROR, format)
#define LOG_WARNING(format) LOG_NO_FORMAT(RECOMP_LOG_WARNING, format)
#define LOG_INFO(format) LOG_NO_FORMAT(RECOMP_LOG_INFO, format)
#define LOG_DEBUG(format) LOG_NO_FORMAT(RECOMP_LOG_DEBUG, format)
#define LOG_VERBOSE(format) LOG_NO_FORMAT(RECOMP_LOG_VERBOSE, format)

// Short-hand Logging Macros:
#define LOGF_F(format, ...) LOG_FATAL_F(format, __VA_ARGS__)
#define LOGE_F(format, ...) LOG_ERROR_F(format, __VA_ARGS__)
#define LOGW_F(format, ...) LOG_WARNING_F(format, __VA_ARGS__)
#define LOGI_F(format, ...) LOG_INFO_F(format, __VA_ARGS__)
#define LOGD_F(format, ...) LOG_DEBUG_F(format, __VA_ARGS__)
#define LOGV_F(format, ...) LOG_VERBOSE_F(format, __VA_ARGS__)

#define LOGF(format) LOG_FATAL(format)
#define LOGE(format) LOG_ERROR(format)
#define LOGW(format) LOG_WARNING(format)
#define LOGI(format) LOG_INFO(format)
#define LOGD(format) LOG_DEBUG(format)
#define LOGV(format) LOG_VERBOSE(format)

#endif