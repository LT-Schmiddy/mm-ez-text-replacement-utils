#include "msg_table.h"
#include "custom_includes/stdbool.h"
#include "custom_includes/stdint.h"



// Entry Functions:
MsgEntry* MsgEntry_Create(u16 textId) {
    MsgEntry* retVal = recomp_alloc(sizeof(MsgEntry));
    retVal->callback = NULL;
    retVal->textId = textId;
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

    recomp_printf("%sCreating cluster with id 0x%02x (%u).\n", LOG_HEADER, (u32)id, (u32)id);
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
    MsgEntry* retVal = cluster->entries[pos];

    if (retVal != NULL && retVal->textId != textId) {
        return NULL;
    }
    return retVal;
}

MsgBuffer* MsgEntryCluster_GetBuffer(MsgEntryCluster* cluster, u16 textId) {    
    MsgEntry* entry = MsgEntryCluster_GetEntry(cluster, textId);

    if (entry == NULL) {
        return NULL;
    }
    return MsgBuffer_Load(entry->buf_store);
}



void MsgEntryCluster_SetBuffer(MsgEntryCluster* cluster, u16 textId, MsgBuffer* entry) {
    SPLIT_TEXT_ID(textId, cl, pos);

    if (cluster->entries[pos] == NULL) {
        cluster->entries[pos] = MsgEntry_Create(textId);
        cluster->entries[pos]->textId = textId;
        cluster->count++;
    }

    // memcpy(&cluster->entries[pos]->buf_store, entry, sizeof(MsgBuffer));
    cluster->entries[pos]->buf_store = MsgBuffer_Store(entry);
}

// Table Functions:
MsgTable* MsgTable_Create() {
    MsgTable* retVal = recomp_alloc(sizeof(MsgTable));
    
    // Setting all clusters to null:
    for (int i = 0; i < UINT8_MAX; i++) {
        retVal->clusters[i] = NULL;
    }
    retVal->cluster_count++;
    recomp_printf("%sMessage Table Created.\n", LOG_HEADER);
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
    if (table->clusters[cl] == NULL) {
        return NULL;
    }

    return MsgEntryCluster_GetEntry(table->clusters[cl], textId);
}

MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 textId) {
    SPLIT_TEXT_ID(textId, cl, pos);
    if (table->clusters[cl] == NULL) {
        return NULL;
    }

    return MsgEntryCluster_GetBuffer(table->clusters[cl], textId);
}

u32 MsgTable_GetBufferLen(MsgTable* table, u16 textId) {
    MsgEntry* entry = MsgTable_GetEntry(table, textId);
    if (entry == NULL) {
        return -1;
    }

    MsgBuffer* buf = MsgBuffer_Load(entry->buf_store);
    u32 len =  MsgBuffer_Len(buf);
    MsgBuffer_Destroy(buf);
    return len;
}

void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry) {
    SPLIT_TEXT_ID(textId, cl, pos);
    if (table->clusters[cl] == NULL) {
        table->clusters[cl] = MsgEntryCluster_Create(cl);
    }
    
    MsgEntryCluster_SetBuffer(table->clusters[cl], textId, entry);
    recomp_printf("%Setting Text Entry Id 0x%04X (%i)\n", LOG_HEADER, (u32)textId, (u32)textId);
}

void MsgTable_SetBufferEmpty(MsgTable* table, u16 textId) {
    MsgBuffer buf;
    // Default Header.
    MsgBuffer_WriteDefaultHeader(&buf);

    // Empty Content:
    MsgSContent_SetEmpty(MsgBuffer_GetContentPtr(&buf));

    // Copy to table:
    MsgTable_SetBuffer(table, textId, &buf);
}

void MsgTable_SetCallback(MsgTable* table, u16 textId, MsgCallback callback) {
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        search->callback = callback;
    } else {
        recomp_printf("%sERROR assigning callback for textId %i - set the buffer first.\n", LOG_HEADER, search->textId);
    }
}

MsgBuffer* MsgTable_RunBufferCallback(MsgTable* table, u16 textId, PlayState* play){
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        MsgBuffer* buf = MsgBuffer_Load(search->buf_store);

        if (search->callback != NULL) {
            recomp_printf("Running Callback for 0x%04x\n", textId);
            search->callback(buf, textId, play);
        }
        return buf;
    }

    return NULL;
}


