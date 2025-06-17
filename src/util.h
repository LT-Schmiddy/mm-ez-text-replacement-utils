#ifndef __UTIL__
#define __UTIL__

#include "mod_globals.h"
#include "libc/string.h"

typedef enum {
    PCAB_NONE,
    PCAB_AS_WIDE_SPECIFIER,
    PCAB_AS_BYTES,

} PrintCharArgBytesHandling;

typedef struct {
    PrintCharArgBytesHandling arg_byte_handling;

    int arg_bytes_max;
    int arg_bytes_remaining;
} PrintCharConfig;

// Allows adjusting the behavior if necessary.
extern PrintCharConfig print_char_config;

char toupper(char c);
char tolower(char c);
bool is_printable_char(char c);
char hex_to_byte(char* str);
void write_byte_to_hex(u8 byte, char* out_str);
void message_id_as_hex(u16 id, char* out_str);

// Invoke to put print_char back in it's default state. Call once in any function that uses `print_char`, prior to usage.
void print_char_config_reset();
void print_char(char character);

#endif