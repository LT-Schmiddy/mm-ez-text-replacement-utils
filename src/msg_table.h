#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"

#define MESSAGE_BUFFER_SIZE 1280
#define MESSAGE_HEADER_SIZE 11
#define MESSAGE_CONTENT_SIZE MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define MSG_TABLE_START_SIZE 16
#define LOG_HEADER "EZ Text Replacer: "

// typedef unsigned long u64;
// typedef long s64;

typedef union {
        char schar[MESSAGE_BUFFER_SIZE]; // msgBuf
        u16 wchar[MESSAGE_BUFFER_SIZE / 2];   // msgBufWide
} MsgBuffer;

typedef struct {
    u16 textId;
    u16 len;
    MsgBuffer buf;
} MsgEntry;

typedef struct {
    MsgEntry* entries;
    u16 count;
    u16 capacity;
    bool _automaticSorting;
} MsgTable;

// Function Declarations:
s32 MsgBuffer_Len(MsgBuffer* buf);
s32 MsgBuffer_CopyFromCharStr(MsgBuffer* dst, char* src);
s32 MsgBuffer_CopyFromCharStr_PipeEscapeBytes(MsgBuffer* dst, char* src);
MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
void MsgTable_Expand(MsgTable* table);
void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id);
s32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry);
void MsgTable_BubbleSort(MsgTable* table);
void MsgTable_Swap(MsgEntry* a, MsgEntry* b);

#endif