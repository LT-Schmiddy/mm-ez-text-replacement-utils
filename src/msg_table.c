#include "msg_table.h"
#include "custom_includes/stdbool.h"
#include "custom_includes/stdint.h"

MsgEntry* MsgEntry_Create(u16 textId) {
    MsgEntry* retVal = recomp_alloc(sizeof(MsgEntry));
    retVal->callback = NULL;
    retVal->textId = textId;
    return retVal;
}

void MsgEntry_Destroy(MsgEntry* entry) {
    recomp_free(entry);
}

MsgTable* MsgTable_Create() {
    MsgTable* retVal = recomp_alloc(sizeof(MsgTable));
    
    // Setting Initial Values:
    retVal->entries = recomp_alloc( MSG_TABLE_START_SIZE * sizeof(MsgEntry*));
    retVal->count = 0;
    retVal->capacity = MSG_TABLE_START_SIZE;
    retVal->_automaticSorting = true;

    recomp_printf("%sMessage Table Created.\n", LOG_HEADER);
    return retVal;
}

void MsgTable_Destroy(MsgTable* table) {
    for (int i = 0; i < table->count; i++) {
        MsgEntry_Destroy(table->entries[i]);
    }
    recomp_free(table->entries);
    recomp_free(table);
}

void MsgTable_Expand(MsgTable* table) {
    MsgTable_ExpandTo(table, table->capacity * 2);
}

void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity) {
    // Create the expanded table:
    MsgEntry** new_entries = recomp_alloc(new_capacity * sizeof(MsgEntry*));
    // Copy to it:
    memcpy(new_entries, table->entries, table->count * sizeof(MsgEntry*));
    // Trash the old table:
    recomp_free(table->entries);
    // Now use the new table:
    table->entries = new_entries;
    table->capacity = new_capacity;

    recomp_printf("%sExpand MsgTable capacity to %d entries.\n", LOG_HEADER, table->capacity);
}

MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id) {
    // Using Binary search. Thanks to the constant sorting, This should be faster than a linear search:
    if (table->count == 0) {
        IF_DEBUG recomp_printf("Empty Lookup:\n");
        return NULL;
    }    
    
    // To avoid bugs, don't use binary search until we've reached a minimum size.
    if (table->count < START_USING_BINARY_LOOKUP) {
        IF_DEBUG recomp_printf("Linear Lookup:\n");
        for (s32 i = 0; i < table->count; i++) {
            IF_DEBUG recomp_printf("\t0x%04X\n", table->entries[i]->textId);
            if (table->entries[i]->textId == id) {
                IF_DEBUG recomp_printf("FOUND ID\n");
                return table->entries[i];
            }
        }
        return NULL;
    } else {
        s32 low = 0;
        s32 high = table->count - 1;
        MsgEntry** entries = table->entries;
        IF_DEBUG recomp_printf("Binary Lookup:\n");
        while (low <= high) {
            int mid = low + (high - low) / 2;
            IF_DEBUG recomp_printf("\tHigh: %d, Low: %d, Central Point: %d Center ID: 0x%04X\n", high, low, mid, (u32)entries[mid]->textId);
        
            if (entries[mid]->textId == id) {
                IF_DEBUG recomp_printf("FOUND ID\n");
                return entries[mid];
                
            }
            else if (entries[mid]->textId < id) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return NULL;
    }
}

MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id) {
    MsgEntry* entry = MsgTable_GetEntry(table, id);
    if (entry == NULL) {
        return NULL;
    }

    return &entry->buf;
}

s32 MsgTable_GetBufferLen(MsgTable* table, u16 id) {
    MsgEntry* entry = MsgTable_GetEntry(table, id);
    if (entry == NULL) {
        return -1;
    }

    return MsgBuffer_Len(&entry->buf);
}

void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry) {
    // Updating Existing Entry:
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        memcpy(&search->buf, entry, sizeof(MsgBuffer));
        return;
    }

    // Handle New Entry:
    if (table->capacity <= table->count) {
        MsgTable_Expand(table);
    }
    IF_DEBUG recomp_printf("correct size\n");
    table->entries[table->count] = MsgEntry_Create(textId);
    search = table->entries[table->count];
    search->textId = textId;
    memcpy(&search->buf, entry, sizeof(MsgBuffer));
    table->count++;
    
    IF_DEBUG recomp_printf("sorting\n");
    if (table->_automaticSorting) {
        MsgTable_BubbleSort(table);
    }

    IF_DEBUG recomp_printf("sorted\n");
    recomp_printf("%sAdding Text Entry Id 0x%04X (%i)\n", LOG_HEADER, (u32)search->textId, (u32)search->textId);
}

void MsgTable_SetBufferEmpty(MsgTable* table, u16 textId) {
    MsgBuffer buf;
    // Default Header.
    MsgBuffer_WriteDefaultHeader(&buf);

    // Empty Content:
    MsgBufferSContent_SetEmpty(MsgBuffer_GetContentPtr(&buf));

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

bool MsgTable_RunCallback(MsgTable* table, u16 textId, PlayState* play){
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        recomp_printf("ID FOUND\n");
        if (search->callback != NULL) {
            recomp_printf("CALLBACK FOUND\n");
            search->callback(play, textId, &search->buf);
            return true;
        }
        else {
            recomp_printf("NO CALLBACK FOUND\n");
        }
    }

    return false;
}


void MsgTable_BubbleSort(MsgTable* table) {
    // Since we're always adding entried to the END of the table, going from greatest to least will be more efficient.
    bool fully_sorted = false;

    while(!fully_sorted) {
        fully_sorted = true;
        
        for (int i = table->count - 1; i >= 1; i--) {
            
            // recomp_printf("Upper: %X (%i), Lower: %X (%i)\n", (u32)upper->textId, (u32)upper->textId, (u32)lower->textId, (u32)lower->textId);
            if (table->entries[i]->textId < table->entries[i - 1]->textId) {
                MsgTable_Swap(&table->entries[i], &table->entries[i - 1]);
                IF_DEBUG recomp_printf("another_pass is needed\n");
                fully_sorted = false;
            }
        }

        IF_DEBUG {
            //print state of list:
            recomp_printf("Current Array: ");
            for (int i = 0; i < table->count; i++) {
                recomp_printf("0x%04X ", table->entries[i]->textId);
            }
            recomp_printf("\n");
        }

    }
}

void MsgTable_Swap(MsgEntry** a, MsgEntry** b) {
    MsgEntry* t;
    
    t = *a;
    *a = *b;
    *b = t;
}
