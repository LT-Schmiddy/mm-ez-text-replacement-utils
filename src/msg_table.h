#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "msg_buffer.h"

#define START_USING_BINARY_LOOKUP 10

#define MSG_TABLE_START_SIZE 16
#define LOG_HEADER "EZ Text Replacer: "

typedef void (*MsgCallback)(PlayState* play, u16 textId, MsgBuffer* buf);

typedef struct {
    u16 textId;
    MsgCallback callback;
    MsgBuffer buf;
} MsgEntry;

typedef struct {
    MsgEntry** entries;
    u16 count;
    u16 capacity;
    bool _automaticSorting;
} MsgTable;

// Function Declarations:
MsgEntry* MsgEntry_Create(u16 textId);
void MsgEntry_Destroy(MsgEntry* entry);
MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
void MsgTable_Expand(MsgTable* table);
void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id);
s32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry);
void MsgTable_SetCallback(MsgTable* table, u16 textId, MsgCallback callback);
bool MsgTable_RunCallback(MsgTable* table, u16 textId, PlayState* play);
void MsgTable_BubbleSort(MsgTable* table);
void MsgTable_Swap(MsgEntry** a, MsgEntry** b);

#endif