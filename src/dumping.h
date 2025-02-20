#ifndef __DUMPING__
#define __DUMPING__

#include "mod_globals.h"
#include "msg_table.h"
#include "libc/string.h"
#include "util.h"

void dump_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf);
void dump_full_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf);

#endif