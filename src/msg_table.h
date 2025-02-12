#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "msg_buffer.h"

#define START_USING_BINARY_LOOKUP 3

#define MSG_TABLE_START_SIZE 16
#define LOG_HEADER "EZ Text Replacer: "

typedef void (*MsgCallback)(PlayState* play, u16 textId, MsgBuffer* buf);

typedef struct {
    u16 textId;
    MsgCallback callback;
    MsgBuffer buf;
} MsgEntry;

typedef struct {
    u8 id;
    u8 count;
    MsgEntry* entries[UINT8_MAX];
} MsgEntryCluster;

typedef struct {
    u8 cluster_count;
    MsgEntryCluster* clusters[UINT8_MAX];
} MsgTable;

// Function Declarations:
MsgEntry* MsgEntry_Create(u16 textId);
void MsgEntry_Destroy(MsgEntry* entry);

MsgEntryCluster* MsgEntryCluster_Create(u8 id);
void MsgEntryCluster_Destroy(MsgEntryCluster* cluster);
MsgEntry* MsgEntryCluster_GetEntry(MsgEntryCluster* cluster, u16 textId);
MsgBuffer* MsgEntryCluster_GetBuffer(MsgEntryCluster* cluster, u16 textId);
void MsgEntryCluster_SetBuffer(MsgEntryCluster* cluster, u16 textId, MsgBuffer* entry);

MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id);
s32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry);
void MsgTable_SetBufferEmpty(MsgTable* table, u16 textId);
void MsgTable_SetCallback(MsgTable* table, u16 textId, MsgCallback callback);
bool MsgTable_RunCallback(MsgTable* table, u16 textId, PlayState* play);


inline void splitTextId(u16 textId, u8* upperId, u8* lowerId) {
    *upperId = (0xFF00 & textId) >> 8;
    *lowerId = (0x00FF & textId);
}
#define SPLIT_TEXT_ID(id, upper, lower) u8 upper, lower; splitTextId(textId, &upper, &lower)
#endif

