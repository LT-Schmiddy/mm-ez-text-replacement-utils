#include "msg_table.h"
#include "custom_includes/stdbool.h"
#include "custom_includes/stdint.h"



// Entry Functions:
MsgEntry* MsgEntry_Create(u16 textId) {
    MsgEntry* retVal = recomp_alloc(sizeof(MsgEntry));
    retVal->callback = NULL;
    retVal->textId = textId;
    retVal->len = 0;
    retVal->buf_store = NULL;
    return retVal;
}

void MsgEntry_Destroy(MsgEntry* entry) {
    recomp_free(entry->buf_store);
    recomp_free(entry);
}

// Cluster Functions:
MsgEntryCluster* MsgEntryCluster_Create(u8 id) {
    MsgEntryCluster* retVal = recomp_alloc(sizeof(MsgEntryCluster));
    retVal->id = id;
    retVal->count = 0;
    // Make sure all entries are null:
    for (int i = 0; i < UINT8_MAX; i++) {
        retVal->entries[i] = NULL;
    }

    LOGD_F("Creating Cluster with Id 0x%02X (%u).", (u32)id, (u32)id);
    return retVal;
}

void MsgEntryCluster_Destroy(MsgEntryCluster* cluster) {
    for (int i = 0; i < UINT8_MAX; i++) {
        if (cluster->entries[i] != NULL) {
            MsgEntry_Destroy(cluster->entries[i]);
        }
    }
    recomp_free(cluster);
}


MsgEntry* MsgEntryCluster_GetEntry(MsgEntryCluster* cluster, u16 textId) {
    SPLIT_TEXT_ID(textId, cl, pos);
    LOGD_F("Loading Entry 0x%02X for 0x%04X", pos, textId);
    MsgEntry* retVal = cluster->entries[pos];

    if (retVal != NULL && retVal->textId != textId) {
        LOGD_F("Entry for 0x%04X is NULL", textId);
        return NULL;
    }
    return retVal;
}

MsgBuffer* MsgEntryCluster_LoadBuffer(MsgEntryCluster* cluster, u16 textId) {    
    MsgEntry* entry = MsgEntryCluster_GetEntry(cluster, textId);

    if (entry == NULL) {
        return NULL;
    }
    // return MsgBuffer_LoadN(entry->buf_store, entry->len);
    return MsgBuffer_CreateFromStr(entry->buf_store);
}

void MsgEntryCluster_StoreBuffer(MsgEntryCluster* cluster, u16 textId, MsgBuffer* entry, MsgCallback callback) {
    SPLIT_TEXT_ID(textId, cl, pos);

    if (cluster->entries[pos] == NULL) {
        cluster->entries[pos] = MsgEntry_Create(textId);
        cluster->entries[pos]->textId = textId;
        cluster->count++;
    }

    cluster->entries[pos]->len = MsgBuffer_Len(entry);
    cluster->entries[pos]->buf_store = MsgBuffer_ShrinkForStorage(entry);
    cluster->entries[pos]->callback = callback;
}

// Table Functions:
MsgTable* MsgTable_Create() {
    MsgTable* retVal = recomp_alloc(sizeof(MsgTable));
    
    // Setting all clusters to null:
    for (int i = 0; i < UINT8_MAX; i++) {
        retVal->clusters[i] = NULL;
    }
    retVal->cluster_count = 0;
    retVal->highest_msg_id = MSG_HIGHEST_ID;
    LOGI("Message Table Created.\n");
    return retVal;
}

void MsgTable_Destroy(MsgTable* table) {
    for (int i = 0; i < UINT8_MAX; i++) {
        if (table->clusters[i] != NULL) {
            MsgEntryCluster_Destroy(table->clusters[i]);
        }
    }
    // recomp_free(table->entries);
    recomp_free(table);
}

MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 textId) {
    SPLIT_TEXT_ID(textId, cl, pos);
    LOGD_F("Loading Cluster 0x%02X for 0x%04X", cl, textId);
    if (table->clusters[cl] == NULL) {
        LOGD_F("Cluster for 0x%04x is NULL", textId);
        return NULL;
    }

    return MsgEntryCluster_GetEntry(table->clusters[cl], textId);
}

MsgBuffer* MsgTable_LoadBuffer(MsgTable* table, u16 textId) {
    SPLIT_TEXT_ID(textId, cl, pos);
    if (table->clusters[cl] == NULL) {
        return NULL;
    }

    return MsgEntryCluster_LoadBuffer(table->clusters[cl], textId);
}

void MsgTable_StoreBuffer(MsgTable* table, u16 textId, MsgBuffer* entry, MsgCallback callback) {
    SPLIT_TEXT_ID(textId, cl, pos);
    if (table->clusters[cl] == NULL) {
        table->clusters[cl] = MsgEntryCluster_Create(cl);
    }
    
    MsgEntryCluster_StoreBuffer(table->clusters[cl], textId, entry, callback);
    LOGD_F("Setting Text Entry Id 0x%04X (%i)", (u32)textId, (u32)textId);
}

void MsgTable_StoreBufferEmpty(MsgTable* table, u16 textId, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    // Copy to table:
    MsgTable_StoreBuffer(table, textId, buf, callback);
    MsgBuffer_Destroy(buf);
}

void MsgTable_StoreNewCustomBuffer(MsgTable* table, CustomMsgHandle handle, MsgBuffer* entry, MsgCallback callback) {
    u16 new_id = ++(table->highest_msg_id);
    MsgTable_StoreBuffer(table, new_id, entry, callback);
    handle(&new_id);
}

void MsgTable_StoreNewCustomBufferEmpty(MsgTable* table, CustomMsgHandle handle, MsgCallback callback) {
    u16 new_id = ++(table->highest_msg_id);
    MsgTable_StoreBufferEmpty(table, new_id, callback);
    handle(&new_id);
}

u32 MsgTable_GetBufferLen(MsgTable* table, u16 textId) {
    MsgEntry* entry = MsgTable_GetEntry(table, textId);
    if (entry == NULL) {
        return -1;
    }

    MsgBuffer* buf = MsgBuffer_CreateFromStr(entry->buf_store);
    u32 len =  MsgBuffer_Len(buf);
    MsgBuffer_Destroy(buf);
    return len;
}

void MsgTable_ChangeCallback(MsgTable* table, u16 textId, MsgCallback callback) {
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        search->callback = callback;
    } else {
        LOGE_F("ERROR assigning callback for textId %i - set the buffer first.\n", search->textId);
    }
}

MsgBuffer* MsgTable_LoadBufferCallback(MsgTable* table, u16 textId, PlayState* play) {
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        LOGD_F("Loading Buffer for 0x%04X", textId);
        MsgBuffer* buf = MsgBuffer_CreateFromStr(search->buf_store);
        IF_LOG_VERBOSE MsgSContent_PrintfLn("%m\xBF", buf);
        
        if (search->callback != NULL) {
            LOGD_F("Running Callback for 0x%04X", textId);
            search->callback(buf, textId, play);
        }
        return buf;
    }

    return NULL;
}


