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
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

// #include <stdbool.h>
// #include <stdint.h>


#include "modding.h"
#include "msg_buffer_printf.h"
#include "util.h"
#include "msg_buffer.h"


// void _putchar(char character) {
//     recomp_printf("%c", character);
// }

// internal buffer output
void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen) {
    if (idx < maxlen) {
        ((char*)buffer)[idx] = character;
    }
}


// internal null output
void _out_null(char character, void* buffer, size_t idx, size_t maxlen) {
    (void)character; (void)buffer; (void)idx; (void)maxlen;
}


// internal _putchar wrapper
void _out_char(char character, void* buffer, size_t idx, size_t maxlen) {
    (void)buffer; (void)idx; (void)maxlen;

    print_char(character);
    
}


// internal output function wrapper
void _out_fct(char character, void* buffer, size_t idx, size_t maxlen) {
    (void)idx; (void)maxlen;
    if (character) {
        // buffer is the output fct pointer
        ((out_fct_wrap_type*)buffer)->fct(character, ((out_fct_wrap_type*)buffer)->arg);
    }
}

// internal vsnprintf
int _MsgSContent_Vsnprintf(out_fct_type out, bool buf_pipe_escaped_bytes, char* buffer_msg, const size_t max_len, const char* format, const size_t max_format_len, va_list va) {
    unsigned int flags, width, precision, n;
    size_t idx = 0U;
    char* buffer = (char*)buffer_msg;
    const char* start = format;
    if (!buffer) {
        // use null output function
        out = _out_null;
    }

    unsigned int fmt_pipe_escaped_bytes = true;
    bool main_should_quit = false;
    while (*format != MSG_ENDING_CHAR && !main_should_quit && (size_t)(format - start) < max_format_len) {
        // format specifier?  %[flags][width][.precision][length]

        if (buf_pipe_escaped_bytes && *format == PIPE_CHAR) {
            // Pipe-escaped byte;
            format++;
            if (*format == PIPE_CHAR) {
                out(PIPE_CHAR, buffer, idx++, max_len);
                format++;
            }
            else {
                char out_char = hex_to_byte((char*)format);
                if (out_char != MSG_ENDING_CHAR) {
                    out(out_char, buffer, idx++, max_len);
                    format++;
                    format++;
                } else {
                    main_should_quit = true;
                }
            }
            continue;
        }
        if (*format != '%') {
            // no
            out(*format, buffer, idx++, max_len);
            format++;
            continue;
        }
        // Insert Pipe Evaluation HERE:
        else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch (*format) {
                case '0': flags |= PF_FLAGS_ZEROPAD; format++; n = 1U; break;
                case '-': flags |= PF_FLAGS_LEFT;    format++; n = 1U; break;
                case '+': flags |= PF_FLAGS_PLUS;    format++; n = 1U; break;
                case ' ': flags |= PF_FLAGS_SPACE;   format++; n = 1U; break;
                case '#': flags |= PF_FLAGS_HASH;    format++; n = 1U; break;
                default :                                   n = 0U; break;
            }
        } while (n);

        // evaluate width field
        width = 0U;
        if (_is_digit(*format)) {
            width = _atoi(&format);
        }
        else if (*format == '*') {
            const int w = pf_va_arg(va, int);
            if (w < 0) {
                flags |= PF_FLAGS_LEFT;    // reverse padding
                width = (unsigned int)-w;
            }
            else {
                width = (unsigned int)w;
            }
            format++;
        }

        // evaluate precision field
        precision = 0U;
        if (*format == '.') {
            flags |= PF_FLAGS_PRECISION;
            format++;
            if (_is_digit(*format)) {
                precision = _atoi(&format);
            }
            else if (*format == '*') {
                const int prec = (int) pf_va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
            case 'l' :
                flags |= PF_FLAGS_LONG;
                format++;
                if (*format == 'l') {
                    flags |= PF_FLAGS_LONG_LONG;
                    format++;
                }
                break;
            case 'h' :
                flags |= PF_FLAGS_SHORT;
                format++;
                if (*format == 'h') {
                    flags |= PF_FLAGS_CHAR;
                    format++;
                }
                break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
            case 't' :
                flags |= (sizeof(ptrdiff_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
                format++;
                break;
#endif
            case 'j' :
                flags |= (sizeof(intmax_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
                format++;
                break;
            case 'z' :
                flags |= (sizeof(size_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
                format++;
                break;
            default :
                break;
        }

        // evaluate specifier
        switch (*format) {
            case 'd' :
            case 'i' :
            case 'u' :
            case 'x' :
            case 'X' :
            case 'o' :
            case 'b' : {
                // set the base
                unsigned int base;
                if (*format == 'x' || *format == 'X') {
                    base = 16U;
                }
                else if (*format == 'o') {
                    base =  8U;
                }
                else if (*format == 'b') {
                    base =  2U;
                }
                else {
                    base = 10U;
                    flags &= ~PF_FLAGS_HASH;   // no hash for dec format
                }
                // uppercase
                if (*format == 'X') {
                    flags |= PF_FLAGS_UPPERCASE;
                }

                // no plus or space flag for u, x, X, o, b
                if ((*format != 'i') && (*format != 'd')) {
                    flags &= ~(PF_FLAGS_PLUS | PF_FLAGS_SPACE);
                }

                // ignore '0' flag when precision is given
                if (flags & PF_FLAGS_PRECISION) {
                    flags &= ~PF_FLAGS_ZEROPAD;
                }

                // convert the integer
                if ((*format == 'i') || (*format == 'd')) {
                    // signed
                    if (flags & PF_FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        const long long value = pf_va_arg(va, long long);
                        idx = _ntoa_long_long(out, buffer, idx, max_len, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
                    }
                    else if (flags & PF_FLAGS_LONG) {
                        const long value = pf_va_arg(va, long);
                        idx = _ntoa_long(out, buffer, idx, max_len, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                    }
                    else {
                        const int value = (flags & PF_FLAGS_CHAR) ? (char) pf_va_arg(va, int) : (flags & PF_FLAGS_SHORT) ? (short int)pf_va_arg(va, int) : pf_va_arg(va, int);
                        idx = _ntoa_long(out, buffer, idx, max_len, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                    }
                }
                else {
                    // unsigned
                    if (flags & PF_FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                        idx = _ntoa_long_long(out, buffer, idx, max_len, pf_va_arg(va, unsigned long long), false, base, precision, width, flags);
#endif
                    }
                    else if (flags & PF_FLAGS_LONG) {
                        idx = _ntoa_long(out, buffer, idx, max_len, pf_va_arg(va, unsigned long), false, base, precision, width, flags);
                    }
                    else {
                        const unsigned int value = (flags & PF_FLAGS_CHAR) ? (unsigned char)pf_va_arg(va, unsigned int) : (flags & PF_FLAGS_SHORT) ? (unsigned short int)pf_va_arg(va, unsigned int) : pf_va_arg(va, unsigned int);
                        idx = _ntoa_long(out, buffer, idx, max_len, value, false, base, precision, width, flags);
                    }
                }
                format++;
                break;
            }
#if defined(PRINTF_SUPPORT_FLOAT)
            case 'f' :
            case 'F' :
                if (*format == 'F') flags |= PF_FLAGS_UPPERCASE;
                idx = _ftoa(out, buffer, idx, max_len, pf_va_arg(va, double), precision, width, flags);
                format++;
                break;
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                if ((*format == 'g')||(*format == 'G')) flags |= PF_FLAGS_ADAPT_EXP;
                if ((*format == 'E')||(*format == 'G')) flags |= PF_FLAGS_UPPERCASE;
                idx = _etoa(out, buffer, idx, max_len, pf_va_arg(va, double), precision, width, flags);
                format++;
                break;
#endif  // PRINTF_SUPPORT_EXPONENTIAL
#endif  // PRINTF_SUPPORT_FLOAT
            case 'c' : {
                unsigned int l = 1U;
                // pre padding
                if (!(flags & PF_FLAGS_LEFT)) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                // char output
                out((char)pf_va_arg(va, int), buffer, idx++, max_len);
                // post padding
                if (flags & PF_FLAGS_LEFT) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                format++;
                break;
            }

            case 's' : {
                const char* p = pf_va_arg(va, char*);
                unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
                // pre padding
                if (flags & PF_FLAGS_PRECISION) {
                    l = (l < precision ? l : precision);
                }
                if (!(flags & PF_FLAGS_LEFT)) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                // string output
                while ((*p != 0) && (!(flags & PF_FLAGS_PRECISION) || precision--)) {
                    out(*(p++), buffer, idx++, max_len);
                }
                // post padding
                if (flags & PF_FLAGS_LEFT) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                format++;
                break;
            }
            // Message Buffer flag:
            case 'n' : 
                fmt_pipe_escaped_bytes = false;
            case 'M' : {
                char* p = pf_va_arg(va, char*);
                unsigned int l = MIN(MsgSContent_Len(p), (precision ? precision : MSG_CONTENT_SIZE));
                // pre padding
                if (flags & PF_FLAGS_PRECISION) {
                    l = (l < precision ? l : precision);
                }
                if (!(flags & PF_FLAGS_LEFT)) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                bool str_should_quit = false;
                while (*p != MSG_ENDING_CHAR && !str_should_quit && (!(flags & PF_FLAGS_PRECISION) || precision--)) {
                    // Handle pipe-escaped byte:
                    if (fmt_pipe_escaped_bytes && *p == PIPE_CHAR) {
                        p++;
                        if (*p == PIPE_CHAR) {
                            out(PIPE_CHAR, buffer, idx++, max_len);
                            p++;
                        }
                        else {
                            char out_char = hex_to_byte((char*)p);
                            if (out_char != MSG_ENDING_CHAR) {
                                out(out_char, buffer, idx++, max_len);
                                p++;
                                p++;
                            } else {
                                str_should_quit = true;
                            }
                        }
                    } else {
                        out(*p, buffer, idx++, max_len);
                        p++;
                    }
                }
                // post padding
                if (flags & PF_FLAGS_LEFT) {
                    while (l++ < width) {
                        out(' ', buffer, idx++, max_len);
                    }
                }
                format++;
                // Set it back to true for next time.
                fmt_pipe_escaped_bytes = true;
                break;
            }

            case 'p' : {
                width = sizeof(void*) * 2U;
                flags |= PF_FLAGS_ZEROPAD | PF_FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
                const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
                if (is_ll) {
                    idx = _ntoa_long_long(out, buffer, idx, max_len, (uintptr_t)pf_va_arg(va, void*), false, 16U, precision, width, flags);
                }
                else {
#endif
                    idx = _ntoa_long(out, buffer, idx, max_len, (unsigned long)((uintptr_t)pf_va_arg(va, void*)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
                }
#endif
                format++;
                break;
            }

            case '%' :
                out('%', buffer, idx++, max_len);
                format++;
                break;

            default :
                out(*format, buffer, idx++, max_len);
                format++;
                break;
        }
    }

    out(MSG_ENDING_CHAR, buffer, idx < max_len ? idx : max_len - 1U, max_len);

    // termination
    // return written chars without terminating \0
    return (int)idx;
}


///////////////////////////////////////////////////////////////////////////////

int MsgSContent_PrintfLn(const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    char buffer[1];
    const int ret = _MsgSContent_Vsnprintf(_out_char, true, buffer, MSG_CONTENT_SIZE, (const char*)format, (size_t)-1, va);
    recomp_printf("\n");
    pf_va_end(va);
    return ret;
}

int MsgSContent_Printf(const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    char buffer[1];
    const int ret = _MsgSContent_Vsnprintf(_out_char, true, buffer, MSG_CONTENT_SIZE, (const char*)format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}

int MsgSContent_Sprintf(char* buffer, const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    const int ret = _MsgSContent_Vsnprintf(_out_buffer, true, buffer, MSG_CONTENT_SIZE, format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}

int MsgSContent_NoPipe_PrintfLn(const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    char buffer[1];
    const int ret = _MsgSContent_Vsnprintf(_out_char, false, buffer, MSG_CONTENT_SIZE, (const char*)format, (size_t)-1, va);
    recomp_printf("\n");
    pf_va_end(va);
    return ret;
}

int MsgSContent_NoPipe_Printf(const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    char buffer[1];
    const int ret = _MsgSContent_Vsnprintf(_out_char, false, buffer, MSG_CONTENT_SIZE, (const char*)format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}

int MsgSContent_NoPipe_Sprintf(char* buffer, const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    const int ret = _MsgSContent_Vsnprintf(_out_buffer, false, buffer, MSG_CONTENT_SIZE, format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}


// int MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...) {
//     va_list va;
//     pf_va_start(va, format);
//     const int ret = _MsgSContent_Vsnprintf(_out_buffer, buffer, count, format, (size_t)-1, va);
//     pf_va_end(va);
//     return ret;
// }


// int MsgSContent_Vprintf(const char* format, va_list va) {
//     char buffer[1];
//     return _MsgSContent_Vsnprintf(_out_char, (char*)buffer, (size_t)-1, format, (size_t)-1, va);
// }


// int MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va) {
//     return _MsgSContent_Vsnprintf(_out_buffer, buffer, count, format, (size_t)-1, va);
// }


// int MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...) {
//     va_list va;
//     pf_va_start(va, format);
//     const out_fct_wrap_type out_fct_wrap = { out, arg };
//     const int ret = _MsgSContent_Vsnprintf(_out_fct, (char*)(uintptr_t)&out_fct_wrap, (size_t)-1, format, (size_t)-1, va);
//     pf_va_end(va);
//     return ret;
// }
