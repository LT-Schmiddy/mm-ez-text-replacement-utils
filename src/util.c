#include "util.h"

static char const _hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char toupper(char c) {
    if (((u8)c) >= 97 && ((u8)c) < 122) {
        return ((u8)c) - 32;
    }
    else { 
        return c;
    }
}

char tolower(char c) {
    if (((u8)c) >= 65 && ((u8)c) < 90) {
        return ((u8)c) + 32;
    }
    else { 
        return c;
    }
}

bool is_printable_char(char c) {
    return ((u8)c) > 31 && ((u8)c) < 127;
}

char hex_to_byte(char* str) {
    u8 retVal = 0;
    
    u8 upper = 0;
    u8 lower = 0;
    
    // recomp_printf("Str: %s\n", str);
    for (u8 i = 0; i < 16; i++) {
        
        if (toupper(str[0]) == _hex[i]) {
            // recomp_printf("Upper Char: %c\n", _hex[i]);
            upper = i;
        }
        if (toupper(str[1]) == _hex[i]) {
            // recomp_printf("Lower Char: %c\n", _hex[i]);
            lower = i;
        }
    }

    return (upper << 4) | lower;
}

void write_byte_to_hex(u8 byte, char* out_str) {
    u8 upper = (byte >> 4) & 0b00001111;
    u8 lower = byte & 0b00001111;

    out_str[0] = _hex[upper];
    out_str[1] = _hex[lower];
}
