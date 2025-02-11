#ifndef __MSG_BUFFER_PRINTF_UTILS__
#define __MSG_BUFFER_PRINTF_UTILS__

#include "custom_includes/stdarg.h"
// #include "libc/stdarg.h"
#include "custom_includes/stddef.h"
// #include "libc/stddef.h"
#include "custom_includes/stdbool.h"
#include "custom_includes/stdint.h"

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE    32U
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_FTOA_BUFFER_SIZE
#define PRINTF_FTOA_BUFFER_SIZE    32U
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif

// define the default floating point precision
// default: 6 digits
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION  6U
#endif

// define the largest float suitable to print with %f
// default: 1e9
#ifndef PRINTF_MAX_FLOAT
#define PRINTF_MAX_FLOAT  1e9
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_SUPPORT_PTRDIFF_T
#endif

///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
#define PF_FLAGS_ZEROPAD   (1U <<  0U)
#define PF_FLAGS_LEFT      (1U <<  1U)
#define PF_FLAGS_PLUS      (1U <<  2U)
#define PF_FLAGS_SPACE     (1U <<  3U)
#define PF_FLAGS_HASH      (1U <<  4U)
#define PF_FLAGS_UPPERCASE (1U <<  5U)
#define PF_FLAGS_CHAR      (1U <<  6U)
#define PF_FLAGS_SHORT     (1U <<  7U)
#define PF_FLAGS_LONG      (1U <<  8U)
#define PF_FLAGS_LONG_LONG (1U <<  9U)
#define PF_FLAGS_PRECISION (1U << 10U)
#define PF_FLAGS_ADAPT_EXP (1U << 11U)

#define pf_va_start(v, l) __builtin_va_start(v, l)
#define pf_va_end(v) __builtin_va_end(v)
#define pf_va_arg(v, l) __builtin_va_arg(v, l)
#define pf_va_copy(d, s) __builtin_va_copy(d, s)

// output function type
typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);


// wrapper (used as buffer) for output function type
typedef struct {
  void  (*fct)(char character, void* arg);
  void* arg;
} out_fct_wrap_type;


/**
 * Output a character to a custom device like UART, used by the printf() function
 * This function is declared here only. You have to write your custom implementation somewhere
 * \param character Character to output
 */
void _putchar(char character);

void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen);
void _out_null(char character, void* buffer, size_t idx, size_t maxlen);
void _out_char(char character, void* buffer, size_t idx, size_t maxlen);
void _out_fct(char character, void* buffer, size_t idx, size_t maxlen);
unsigned int _strnlen_s(const char* str, size_t maxsize);
bool _is_digit(char ch);
unsigned int _atoi(const char** str);
size_t _out_rev(out_fct_type out, char* buffer, size_t idx, size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags);
size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags);
size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags);

#if defined(PRINTF_SUPPORT_LONG_LONG)
size_t _ntoa_long_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags);
#endif

#if defined(PRINTF_SUPPORT_FLOAT)
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
// forward declaration so that _ftoa can switch to exp notation for values > PRINTF_MAX_FLOAT
size_t _etoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags);
#endif
size_t _ftoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags);

#endif


#endif
