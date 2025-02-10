#include "msg_table.h"

MsgEntry* MsgEntry_Create(u16 textId) {
    MsgEntry* retVal = recomp_alloc(sizeof(MsgEntry));
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
            IF_DEBUG recomp_printf("\t%i\n", table->entries[i]->textId);
            if (table->entries[i]->textId == id) {
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
            // recomp_printf("High: %d, Low: %d, Central Point: %d\n", high, low, mid);
        
            if (entries[mid]->textId == id) {
                IF_DEBUG recomp_printf("\thigh: %d, Low: %d, Central Point: %d\n", high, low, mid);
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
    
    table->entries[table->count] = MsgEntry_Create(textId);
    search = table->entries[table->count];
    search->textId = textId;
    memcpy(&search->buf, entry, sizeof(MsgBuffer));
    table->count++;

    if (table->_automaticSorting) {
        MsgTable_BubbleSort(table);
    }

    recomp_printf("%sAdding text entry id %i\n", LOG_HEADER, search->textId);
}


void MsgTable_BubbleSort(MsgTable* table) {
    // Since we're always adding entried to the END of the table, going from greatest to least will be more efficient.
    MsgEntry** entries = table->entries;
    bool fully_sorted = false;

    while(!fully_sorted) {
        fully_sorted = true;
        for (int i = table->count - 1; i >= 1; i--) {
            MsgEntry* upper = entries[i];
            MsgEntry* lower = entries[i - 1];

            if (upper->textId < lower->textId) {
                MsgTable_Swap(&upper, &lower);
                fully_sorted = false;
            }
        }
    }
}

void MsgTable_Swap(MsgEntry** a, MsgEntry** b) {
    MsgEntry* t;
    memcpy(&t, a, sizeof(MsgEntry*));
    memcpy(a, b, sizeof(MsgEntry*));
    memcpy(b, &t, sizeof(MsgEntry*));
}
