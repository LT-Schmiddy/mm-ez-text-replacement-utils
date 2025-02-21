#ifndef __MSG_TABLE__
#define __MSG_TABLE__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "msg_buffer.h"

#define MSG_HIGHEST_ID 0x354C
#define LOG_HEADER "EZ Text Replacer: "

#define CUSTOM_MSG_HANDLE(name) EZTR_CustomMsgHandle_##name



#define DEFINE_CUSTOM_MSG_HANDLE(name) RECOMP_EXPORT CUSTOM_MSG_HANDLE_NO_EXPORT(name)
#define DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT(name) \
    u16 CUSTOM_MSG_HANDLE(name)(u16* new_id) \
    { static u16 id; if (new_id != NULL) { id = *new_id; } return id; }

    #define EXTERN_CUSTOM_MSG_HANDLE(name) u16 CUSTOM_MSG_HANDLE(name)(u16* new_id)
#define CUSTOM_MSG_ID(name) CUSTOM_MSG_HANDLE(name)(NULL)

// Type of the custom message handle:
typedef u16 (*CustomMsgHandle)(u16* new_id);

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
    u16 highest_msg_id;
    MsgEntryCluster* clusters[UINT8_MAX];
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
void MsgTable_StoreBuffer(MsgTable* table, u16 textId, MsgBuffer* entry, MsgCallback callback);
void MsgTable_StoreBufferEmpty(MsgTable* table, u16 textId, MsgCallback callback);
MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id);
MsgBuffer* MsgTable_LoadBuffer(MsgTable* table, u16 id);
u32 MsgTable_GetBufferLen(MsgTable* table, u16 id);
void MsgTable_ChangeCallback(MsgTable* table, u16 textId, MsgCallback callback);
MsgBuffer* MsgTable_LoadBufferCallback(MsgTable* table, u16 textId, PlayState* play);

inline void splitTextId(u16 textId, u8* upperId, u8* lowerId) {
    *upperId = (0xFF00 & textId) >> 8;
    *lowerId = (0x00FF & textId);
}
#define SPLIT_TEXT_ID(id, upper, lower) u8 upper, lower; splitTextId(textId, &upper, &lower)
#endif

