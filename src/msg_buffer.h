#ifndef __MSG_BUFFER__
#define __MSG_BUFFER__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "enums.h"
#include "msg_buffer_printf.h"

typedef union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
        u64 force_structure_alignment_msg;
} MsgBuffer;

u32 MsgBuffer_Len(MsgBuffer* buf);
u32 MsgBuffer_ContentLen(MsgBuffer* buf);
u32 MsgBuffer_WriteFromStr(MsgBuffer* dst, char* src);
u32 MsgBuffer_WriteFromStr_PipeEscapeBytes(MsgBuffer* dst, char* src);
void MsgBuffer_WriteDefaultHeader(MsgBuffer* buf);

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

// Msg Content:
MsgSContent* MsgBuffer_GetContentPtr(MsgBuffer* buf);
char* MsgBuffer_GetContentCharPtr(MsgBuffer* buf);
void MsgSContent_SetEmpty(MsgSContent* cont);
u32 MsgSContent_Len(MsgSContent* cont);

#endif