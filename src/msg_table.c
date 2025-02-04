#include "msg_table.h"
#include "libc/string.h"
// Patches a function in the base game that's used to check if the player should quickspin.

MsgTable* MsgTable_Create() {
    MsgTable* retVal = recomp_alloc(sizeof(MsgTable));
    
    // Setting Initial Values:
    retVal->entries = recomp_alloc( MSG_TABLE_START_SIZE * sizeof(MsgEntry));
    retVal->count = 0;
    retVal->capacity = MSG_TABLE_START_SIZE;
    retVal->automaticSorting = true;

    recomp_printf("%sMessage Table Created.\n", LOG_HEADER);
    return retVal;
}

void MsgTable_Destroy(MsgTable* table) {
    recomp_free(table->entries);
    recomp_free(table);
}

void MsgTable_Expand(MsgTable* table) {
    MsgTable_ExpandTo(table, table->capacity * 2);
}

void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity) {
    // Create the expanded table:
    MsgEntry* new_entries = recomp_alloc(new_capacity * sizeof(MsgEntry));
    // Copy to it:
    memcpy(new_entries, table->entries, table->count * sizeof(MsgEntry));
    // Trash the old table:
    recomp_free(table->entries);
    // Now use the new table:
    table->entries = new_entries;
    table->capacity = new_capacity;

    recomp_printf("%sExpand MsgTable capacity to %d entries.\n", LOG_HEADER, table->capacity);
}

void MsgTable_AddEntry(MsgTable* table, u16 textId, char* text) {
    if (table->capacity <= table->count) {
        MsgTable_Expand(table);
    }

    table->entries[table->count].textId = textId;
    strcpy(table->entries[table->count].buf.schar, text);
    table->count++;

    if (table->automaticSorting) {
        MsgTable_BubbleSort(table);
    }

    recomp_printf("%sAdding text entry: %s\n", LOG_HEADER, text);
}


void MsgTable_BubbleSort(MsgTable* table) {
    // Since we're always adding entried to the END of the table, going from greatest to least will be more efficient.
    MsgEntry* entries = table->entries;
    bool fully_sorted = false;

    while(!fully_sorted) {
        fully_sorted = true;
        for (int i = table->count - 1; i >= 1; i--) {
            MsgEntry* upper = &entries[i];
            MsgEntry* lower = &entries[i - 1];

            if (upper->textId < lower->textId) {
                MsgTable_Swap(upper, lower);
                fully_sorted = false;
            }
        }
    }
}

void MsgTable_Swap(MsgEntry* a, MsgEntry* b) {
    MsgEntry t;

    memcpy(&t, a, sizeof(MsgEntry));
    memcpy(a, b, sizeof(MsgEntry));
    memcpy(b, &t, sizeof(MsgEntry));
}