#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "msg_buffer.h"
#include "msg_table_custom_handles.h"

// #define LOG_HEADER "EZ Text Replacer: "


// Runtime callback:
typedef void (*MsgCallback)(MsgBuffer* buf, u16 textId, PlayState* play);

typedef struct {
    u16 textId;
    MsgCallback callback;
    size_t len;
    char* buf_store;
} MsgEntry;

typedef struct {
    u8 id;
    u8 count;
    MsgEntry* entries[UINT8_MAX];
} MsgEntryCluster;

typedef struct {
    u8 cluster_count;
    MsgEntryCluster* clusters[UINT8_MAX];
    bool locked;
} MsgTable;

// Function Declarations:
MsgEntry* MsgEntry_Create(u16 textId);
void MsgEntry_Destroy(MsgEntry* entry);

MsgEntryCluster* MsgEntryCluster_Create(u8 id);
void MsgEntryCluster_Destroy(MsgEntryCluster* cluster);
MsgEntry* MsgEntryCluster_GetEntry(MsgEntryCluster* cluster, u16 textId);
MsgBuffer* MsgEntryCluster_LoadBuffer(MsgEntryCluster* cluster, u16 textId);
void MsgEntryCluster_StoreBuffer(MsgEntryCluster* cluster, u16 textId, MsgBuffer* entry, MsgCallback callback);

MsgTable* MsgTable_Create();
void MsgTable_Destroy(MsgTable* tbl);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_LoadBuffer(MsgTable* table, u16 id);
void MsgTable_StoreBuffer(MsgTable* table, u16 textId, MsgBuffer* entry, MsgCallback callback);
void MsgTable_StoreBufferEmpty(MsgTable* table, u16 textId, MsgCallback callback);
// bool MsgTable_StoreNewCustomBuffer(MsgTable* table, CustomMsgHandle handle, MsgBuffer* entry, MsgCallback callback);
// bool MsgTable_StoreNewCustomBufferEmpty(MsgTable* table, CustomMsgHandle handle, MsgCallback callback);
u32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_ChangeCallback(MsgTable* table, u16 textId, MsgCallback callback);
MsgBuffer* MsgTable_LoadBufferCallback(MsgTable* table, u16 textId, PlayState* play);

inline void splitTextId(u16 textId, u8* upperId, u8* lowerId) {
    *upperId = (0xFF00 & textId) >> 8;
    *lowerId = (0x00FF & textId);
}
#define SPLIT_TEXT_ID(id, upper, lower) u8 upper, lower; splitTextId(textId, &upper, &lower)
#endif

