///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources.
//        Use this instead of bloated standard/newlib printf.
//        These routines are thread safe and reentrant.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PRINTF_H_
#define _PRINTF_H_

#include "custom_includes/stdarg.h"
// #include "libc/stdarg.h"
#include "custom_includes/stddef.h"
// #include "libc/stddef.h"
#include "msg_buffer_printf_utils.h"

// define this globally (e.g. gcc -DPRINTF_INCLUDE_CONFIG_H ...) to include the
// printf_config.h header file
// default: undefined
#ifdef PRINTF_INCLUDE_CONFIG_H
#include "printf_config.h"
#endif

#define MSG_BUFFER_SIZE 1280
#define MSG_BUFFER_WIDE_SIZE 640
#define MSG_HEADER_SIZE 11
#define MSG_CONTENT_SIZE 1269 // MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define MSG_ENDING_CHAR '\xBF'
#define PIPE_CHAR '|'
// typedef char MsgSContent [MSG_CONTENT_SIZE];

// Internal Functions:
// void _putchar(char character);

void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen);
void _out_null(char character, void* buffer, size_t idx, size_t maxlen);
void _out_char(char character, void* buffer, size_t idx, size_t maxlen);
void _out_fct(char character, void* buffer, size_t idx, size_t maxlen);
// int _MsgSContent_Vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va);
int _MsgSContent_Vsnprintf(out_fct_type out, bool buf_pipe_escaped_bytes, char* buffer, const size_t max_len, const char* format, const size_t max_format_len, va_list va);

/**
 * Tiny printf implementation
 * You have to implement _putchar if you use printf()
 * To avoid conflicts with the regular printf() API it is overridden by macro defines
 * and internal underscore-appended functions like printf_() are used
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
// #define printf printf_
int MsgSContent_PrintfLn(const char* format, ...);
int MsgSContent_Printf(const char* format, ...);

int MsgSContent_NoPipe_PrintfLn(const char* format, ...);
int MsgSContent_NoPipe_Printf(const char* format, ...);


/**
 * Tiny sprintf implementation
 * Due to security reasons (buffer overflow) YOU SHOULD CONSIDER USING (V)SNPRINTF INSTEAD!
 * \param buffer A pointer to the buffer where to store the formatted string. MUST be big enough to store the output!
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */
// #define sprintf sprintf_
int MsgSContent_NoPipe_Sprintf(char* buffer, const char* format, ...);



/**
 * Tiny snprintf/vsnprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 *         If the formatted string is truncated the buffer size (count) is returned
 */
// #define snprintf  snprintf_
// #define vsnprintf vsnprintf_
// int  MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...);
// int MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va);


/**
 * Tiny vprintf implementation
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */
// #define vprintf vprintf_
// int MsgSContent_Vprintf(const char* format, va_list va);


/**
 * printf with output function
 * You may use this as dynamic alternative to printf() with its fixed _putchar() output
 * \param out An output function which takes one character and an argument pointer
 * \param arg An argument pointer for user data passed to output function
 * \param format A string that specifies the format of the output
 * \return The number of characters that are sent to the output function, not counting the terminating null character
 */
// int MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);


#endif  // _PRINTF_H_
