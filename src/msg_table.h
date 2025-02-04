#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"

#define MSG_TABLE_START_SIZE 1
#define LOG_HEADER "EZ Text Replacer: "

// typedef unsigned long u64;
// typedef long s64;

typedef union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
} MsgBuffer;

typedef struct {
    u16 textId;
    MsgBuffer buf;
} MsgEntry;

typedef struct {
    MsgEntry* entries;
    u16 count;
    u16 capacity;
    bool _automaticSorting;
} MsgTable;

// Function Declarations:
MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
void MsgTable_Expand(MsgTable* table);
void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity);
MsgBuffer* MsgTable_GetEntry(MsgTable* table, u16 id);
void MsgTable_SetEntry(MsgTable* table, u16 textId, char* text);
void MsgTable_BubbleSort(MsgTable* table);
void MsgTable_Swap(MsgEntry* a, MsgEntry* b);

#endif