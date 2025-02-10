#ifndef __MSG_BUFFER__
#define __MSG_BUFFER__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "enums.h"

#define MSG_BUFFER_SIZE 1280
#define MSG_HEADER_SIZE 11
#define MSG_CONTENT_SIZE 1269 // MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define MSG_ENDING_CHAR '\xBF'
#define PIPE_CHAR '|'

typedef char MsgBufferSContent [MSG_CONTENT_SIZE];

typedef union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
        u64 force_structure_alignment_msg;
} MsgBuffer;

s32 MsgBuffer_Len(MsgBuffer* buf);
s32 MsgBuffer_ContentLen(MsgBuffer* buf);
s32 MsgBuffer_CreateFromStr(MsgBuffer* dst, char* src);
s32 MsgBuffer_CreateFromStr_PipeEscapeBytes(MsgBuffer* dst, char* src);

// Getters/Setters for the header:
u8 MsgBuffer_GetTextBoxType(MsgBuffer* buf);
void MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type);
u8 MsgBuffer_GetTextBoxYPos(MsgBuffer* buf);
void MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos);
u8 MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf);
void MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon);
u16 MsgBuffer_GetNextMsg(MsgBuffer* buf);
void MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 type);
u16 MsgBuffer_GetFirstItemRupees(MsgBuffer* buf);
void MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 pos);
u16 MsgBuffer_GetSecondItemRupees(MsgBuffer* buf);
void MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 pos);
MsgBufferSContent* MsgBuffer_GetContentPtr(MsgBuffer* buf);

#endif