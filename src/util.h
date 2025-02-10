#ifndef __UTIL__
#define __UTIL__

#include "mod_globals.h"
#include "libc/string.h"

char toupper(char c);
char tolower(char c);
bool is_printable_char(char c);
char hex_to_byte(char* str);
void write_byte_to_hex(u8 byte, char* out_str);

#endif