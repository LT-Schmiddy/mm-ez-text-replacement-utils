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

typedef struct {
    char* name;
    char* color;

} RecompLogLevelInfo;


// ANSI Color codes sourced from here:
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
#define _CRESET "\e[0m"
static const RecompLogLevelInfo _RECOMP_LOG_LEVEL_INFO[] = {
    {"NONE", _CRESET},
    {"FATAL", RECOMP_LOG_CONFIG_FATAL_COLOR},
    {"ERROR", RECOMP_LOG_CONFIG_ERROR_COLOR},
    {"WARNING", RECOMP_LOG_CONFIG_WARNING_COLOR},
    {"INFO", RECOMP_LOG_CONFIG_INFO_COLOR},
    {"DEBUG", RECOMP_LOG_CONFIG_DEBUG_COLOR},
    {"VERBOSE", RECOMP_LOG_CONFIG_VERBOSE_COLOR},
};


#define _LOG_PRINTF_CALL(...) RECOMP_LOG_CONFIG_PRINTF_FUNC(__VA_ARGS__)

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
    _LOG_PRINTF_CALL("%s%s - %s (%s at line %u, in %s): ", _RECOMP_LOG_LEVEL_INFO[log_level].color, RECOMP_LOG_CONFIG_HEADER, \
        _RECOMP_LOG_LEVEL_INFO[log_level].name, __FILE_NAME__, __LINE__, __func__);

#define _LOG_FOOTER _LOG_PRINTF_CALL("%s\n", _CRESET);

#define LOG_FORMAT(log_level, ...) \
    { IF_LOG_LEVEL(log_level) { \
        _LOG_HEADER(log_level) \
        _LOG_PRINTF_CALL( __VA_ARGS__); \
        _LOG_FOOTER \
    }};

// Logging Macros:
#define LOG_FATAL(...) LOG_FORMAT(RECOMP_LOG_FATAL, __VA_ARGS__)
#define LOG_ERROR(...) LOG_FORMAT(RECOMP_LOG_ERROR, __VA_ARGS__)
#define LOG_WARNING(...) LOG_FORMAT(RECOMP_LOG_WARNING, __VA_ARGS__)
#define LOG_INFO(...) LOG_FORMAT(RECOMP_LOG_INFO, __VA_ARGS__)
#define LOG_DEBUG(...) LOG_FORMAT(RECOMP_LOG_DEBUG, __VA_ARGS__)
#define LOG_VERBOSE(...) LOG_FORMAT(RECOMP_LOG_VERBOSE, __VA_ARGS__)

// Short-hand Logging Macros:
#define LOGF(...) LOG_FATAL(__VA_ARGS__)
#define LOGE(...) LOG_ERROR(__VA_ARGS__)
#define LOGW(...) LOG_WARNING(__VA_ARGS__)
#define LOGI(...) LOG_INFO(__VA_ARGS__)
#define LOGD(...) LOG_DEBUG(__VA_ARGS__)
#define LOGV(...) LOG_VERBOSE(__VA_ARGS__)

#endif