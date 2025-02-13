#ifndef __DUMPING__
#define __DUMPING__

#include "mod_globals.h"
#include "msg_table.h"
#include "msg_table_quicksort.h"
#include "libc/string.h"
#include "util.h"

extern const char* eztr_textbox_names[];
extern const char* eztr_icon_names[];

void dump_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf);
void dump_full_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf);

#endif