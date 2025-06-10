#include "util.h"
#include "msg_control_codes.h"
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
    
    LOGV("Str: %c%c\n", str, str + 1);
    for (u8 i = 0; i < 16; i++) {
        
        if (toupper(str[0]) == _hex[i]) {
            LOGV("Upper Char: %c\n", _hex[i]);
            upper = i;
        }
        if (toupper(str[1]) == _hex[i]) {
            LOGV("Lower Char: %c\n", _hex[i]);
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

void message_id_as_hex(u16 id, char* out_str) {
    u8 id_parts[2];
    memcpy(&id_parts, &id, sizeof(u16));
    
    write_byte_to_hex(id_parts[0], &out_str[0]);
    write_byte_to_hex(id_parts[1], &out_str[2]);
}


void print_char(char character) {
    if (is_printable_char(character)) {
        recomp_printf("%c", character);
    } else {
        if (recomp_get_config_u32("text_dumping_cc_macros") && msg_control_code_names[(u8)character] != NULL) {
            recomp_printf("\" %s \"", msg_control_code_names[(u8)character]);
        } else if (recomp_get_config_u32("text_dumping_byte_format")) {
            char out_str[9]= " \"\\x00\" ";
            write_byte_to_hex(character,&out_str[2]);
            recomp_printf("%s", out_str);
        } else {
            char out_str[4] = "|00";
            write_byte_to_hex(character,&out_str[1]);
            recomp_printf("%s", out_str);
        }
    }
}