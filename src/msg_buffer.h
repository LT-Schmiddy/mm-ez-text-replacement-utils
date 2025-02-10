#ifndef __MSG_BUFFER__
#define __MSG_BUFFER__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "enums.h"

#define MESSAGE_BUFFER_SIZE 1280
#define MESSAGE_HEADER_SIZE 11
#define MESSAGE_CONTENT_SIZE 1269 // MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define MSG_ENDING_CHAR '\xBF'
#define PIPE_CHAR '|'

typedef char MsgBufferSContent [MESSAGE_CONTENT_SIZE];

typedef union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
        u64 force_structure_alignment_msg;
} MsgBuffer;

s32 MsgBuffer_Len(MsgBuffer* buf);
s32 MsgBuffer_CreateFromStr(MsgBuffer* dst, char* src);
s32 MsgBuffer_CreateFromStr_PipeEscapeBytes(MsgBuffer* dst, char* src);

#endif