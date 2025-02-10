#include "msg_buffer.h"

s32 MsgBuffer_Len(MsgBuffer* buf) {
    int i = 0;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MESSAGE_BUFFER_SIZE) { 
        i++;
    }

    return i;
}

s32 MsgBuffer_ContentLen(MsgBuffer* buf) {
    int i = MESSAGE_HEADER_SIZE;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MESSAGE_BUFFER_SIZE) {
        i++;
    }

    return i;
}

s32 MsgBuffer_CreateFromStr(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying: ");
    int i = 0;

    bool should_end = false;
    while (!should_end && i < MESSAGE_CONTENT_SIZE) {

        IF_DEBUG recomp_printf( is_printable_char(src[i]) ? "%c" : "\\x%02X", src[i]);
        dst->schar[i + MESSAGE_HEADER_SIZE] = src[i];
        i++;

        should_end = src[i] == MSG_ENDING_CHAR;
    }
    
    IF_DEBUG recomp_printf(" -> %i\n", i); 
    // Add 1 to go from index to length. 
    return i + 1;
}

s32 MsgBuffer_CreateFromStr_PipeEscapeBytes(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying w/ Pipes: ");
    int src_pos = 0;
    int dst_pos = 0;
    bool should_end = false;
    while (!should_end && dst_pos < MESSAGE_CONTENT_SIZE) {

        if (src[src_pos] != PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
            IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
            should_end = src[src_pos] == MSG_ENDING_CHAR;
            src_pos++;
            dst_pos++;
        } else if (src[src_pos + 1] == PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = PIPE_CHAR;
            src_pos += 2;
            dst_pos++;
        } else {
            // Parsing Byte Escape:
            IF_DEBUG recomp_printf("|%c%c", src[src_pos + 1], src[src_pos + 2]);
            char write_byte = hex_to_byte(&src[src_pos + 1]);
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = write_byte;
            should_end = write_byte == MSG_ENDING_CHAR;
            src_pos += 3;
            dst_pos++;
        }

    }
    // Copy Final Char:
    IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
    // dst->schar[src_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
    
    IF_DEBUG recomp_printf(" -> %i\n", src_pos); 
    // Add 1 to go from index to length. 
    return dst_pos + 1;
}

// Getters/Setters for the header:
u8 MsgBuffer_GetTextBoxType(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[0], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type) {
    memcpy(&buf->schar[0], &type, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxYPos(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[1], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos) {
    memcpy(&buf->schar[1], &pos, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[2], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon) {
    memcpy(&buf->schar[2], &icon, sizeof(u8));
}


u16 MsgBuffer_GetNextMsg(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[3], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 type) {
    memcpy(&buf->schar[3], &type, sizeof(u16));
}


u16 MsgBuffer_GetFirstItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[5], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 pos) {
    memcpy(&buf->schar[5], &pos, sizeof(u16));
}


u16 MsgBuffer_GetSecondItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[7], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 pos) {
    memcpy(&buf->schar[7], &pos, sizeof(u16));
}


MsgBufferSContent* MsgBuffer_GetContentPtr(MsgBuffer* buf) {
   return (MsgBufferSContent*)&buf->schar[MESSAGE_HEADER_SIZE];
}
/*
// MsgBuffer Printf Implementation:
int MsgBufferSContent_Vsnprintf(out_fct_type out, char *buffer, const size_t maxlen, const char *format, va_list va) {
    unsigned int flags, width, precision, n;
    size_t idx = 0U;

    if (!buffer) {
        // use null output function
        out = _out_null;
    }

    while (*format) {
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%') {
            // no
            out(*format, buffer, idx++, maxlen);
            format++;
            continue;
        }
        else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch (*format)
            {
            case '0':
                flags |= PF_FLAGS_ZEROPAD;
                format++;
                n = 1U;
                break;
            case '-':
                flags |= PF_FLAGS_LEFT;
                format++;
                n = 1U;
                break;
            case '+':
                flags |= PF_FLAGS_PLUS;
                format++;
                n = 1U;
                break;
            case ' ':
                flags |= PF_FLAGS_SPACE;
                format++;
                n = 1U;
                break;
            case '#':
                flags |= PF_FLAGS_HASH;
                format++;
                n = 1U;
                break;
            default:
                n = 0U;
                break;
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
                flags |= PF_FLAGS_LEFT; // reverse padding
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
                const int prec = (int)pf_va_arg(va, int);
                precision = prec > 0 ? (unsigned int)prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch (*format) {
        case 'l':
            flags |= PF_FLAGS_LONG;
            format++;
            if (*format == 'l') {
                flags |= PF_FLAGS_LONG_LONG;
                format++;
            }
            break;
        case 'h':
            flags |= PF_FLAGS_SHORT;
            format++;
            if (*format == 'h') {
                flags |= PF_FLAGS_CHAR;
                format++;
            }
            break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
        case 't':
            flags |= (sizeof(ptrdiff_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
            format++;
            break;
#endif
        case 'j':
            flags |= (sizeof(intmax_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
            format++;
            break;
        case 'z':
            flags |= (sizeof(size_t) == sizeof(long) ? PF_FLAGS_LONG : PF_FLAGS_LONG_LONG);
            format++;
            break;
        default:
            break;
        }

        // evaluate specifier
        switch (*format) {
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
        case 'b':
        {
            // set the base
            unsigned int base;
            if (*format == 'x' || *format == 'X') {
                base = 16U;
            }
            else if (*format == 'o') {
                base = 8U;
            }
            else if (*format == 'b') {
                base = 2U;
            }
            else {
                base = 10U;
                flags &= ~PF_FLAGS_HASH; // no hash for dec format
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
                    idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
                }
                else if (flags & PF_FLAGS_LONG) {
                    const long value = pf_va_arg(va, long);
                    idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                }
                else {
                    const int value = (flags & PF_FLAGS_CHAR) ? (char)pf_va_arg(va, int) : (flags & PF_FLAGS_SHORT) ? (short int)pf_va_arg(va, int)
                                                                                                                    : pf_va_arg(va, int);
                    idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
                }
            }
            else {
                // unsigned
                if (flags & PF_FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
                    idx = _ntoa_long_long(out, buffer, idx, maxlen, pf_va_arg(va, unsigned long long), false, base, precision, width, flags);
#endif
                }
                else if (flags & PF_FLAGS_LONG) {
                    idx = _ntoa_long(out, buffer, idx, maxlen, pf_va_arg(va, unsigned long), false, base, precision, width, flags);
                }
                else {
                    const unsigned int value = (flags & PF_FLAGS_CHAR) ? (unsigned char)pf_va_arg(va, unsigned int) : (flags & PF_FLAGS_SHORT) ? (unsigned short int)pf_va_arg(va, unsigned int)
                                                                                                                                               : pf_va_arg(va, unsigned int);
                    idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
                }
            }
            format++;
            break;
        }
#if defined(PRINTF_SUPPORT_FLOAT)
        case 'f':
        case 'F':
            if (*format == 'F')
                flags |= PF_FLAGS_UPPERCASE;
            idx = _ftoa(out, buffer, idx, maxlen, pf_va_arg(va, double), precision, width, flags);
            format++;
            break;
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if ((*format == 'g') || (*format == 'G'))
                flags |= PF_FLAGS_ADAPT_EXP;
            if ((*format == 'E') || (*format == 'G'))
                flags |= PF_FLAGS_UPPERCASE;
            idx = _etoa(out, buffer, idx, maxlen, pf_va_arg(va, double), precision, width, flags);
            format++;
            break;
#endif // PRINTF_SUPPORT_EXPONENTIAL
#endif // PRINTF_SUPPORT_FLOAT
        case 'c':
        {
            unsigned int l = 1U;
            // pre padding
            if (!(flags & PF_FLAGS_LEFT)) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            // char output
            out((char)pf_va_arg(va, int), buffer, idx++, maxlen);
            // post padding
            if (flags & PF_FLAGS_LEFT) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            format++;
            break;
        }

        case 's':
        {
            const char *p = pf_va_arg(va, char *);
            unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
            // pre padding
            if (flags & PF_FLAGS_PRECISION) {
                l = (l < precision ? l : precision);
            }
            if (!(flags & PF_FLAGS_LEFT)) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            // string output
            while ((*p != 0) && (!(flags & PF_FLAGS_PRECISION) || precision--)) {
                out(*(p++), buffer, idx++, maxlen);
            }
            // post padding
            if (flags & PF_FLAGS_LEFT) {
                while (l++ < width) {
                    out(' ', buffer, idx++, maxlen);
                }
            }
            format++;
            break;
        }

        case 'p':
        {
            width = sizeof(void *) * 2U;
            flags |= PF_FLAGS_ZEROPAD | PF_FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
            const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
            if (is_ll) {
                idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)pf_va_arg(va, void *), false, 16U, precision, width, flags);
            }
            else
            {
#endif
                idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)pf_va_arg(va, void *)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
            }
#endif
            format++;
            break;
        }

        case '%':
            out('%', buffer, idx++, maxlen);
            format++;
            break;

        default:
            out(*format, buffer, idx++, maxlen);
            format++;
            break;
        }
    }

    // termination
    out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

    // return written chars without terminating \0
    return (int)idx;
}

int MsgBufferSContent_Sprintf(MsgBufferSContent* buffer, const char* format, ...) {
  va_list va;
  pf_va_start(va, format);
  const int ret = MsgBufferSContent_Vsnprintf(_out_buffer, buffer, MESSAGE_CONTENT_SIZE, format, va);
  pf_va_end(va);
  return ret;
}

*/
