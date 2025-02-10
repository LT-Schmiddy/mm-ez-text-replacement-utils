#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "msg_buffer.h"

#define START_USING_BINARY_LOOKUP 3

#define MSG_TABLE_START_SIZE 16
#define LOG_HEADER "EZ Text Replacer: "


typedef struct {
    u16 textId;
    // u16 len;
    MsgBuffer buf;
} MsgEntry;

typedef struct {
    MsgEntry** entries;
    u16 count;
    u16 capacity;
    bool _automaticSorting;
} MsgTable;

// Function Declarations:
MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
void MsgTable_Expand(MsgTable* table);
void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id);
s32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry);
void MsgTable_BubbleSort(MsgTable* table);
void MsgTable_Swap(MsgEntry** a, MsgEntry** b);

#endif