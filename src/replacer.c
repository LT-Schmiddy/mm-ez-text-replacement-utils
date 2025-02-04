#include "mod_globals.h"
#include "msg_table.h"
#include "msg_table_quicksort.h"

MsgTable* table = NULL;

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    table = MsgTable_Create();
    // table->automaticSorting = false;

    MsgTable_SetEntry(table, 0x0005, "Hello First Entry");
    MsgTable_SetEntry(table, 0x0004, "Adding Second Entry");
    MsgTable_SetEntry(table, 0x0003, "Adding Third Entry");
    MsgTable_SetEntry(table, 0x0002, "Adding Fourth Entry");
    MsgTable_SetEntry(table, 0x0001, "Adding Fifth Entry");
    MsgTable_SetEntry(table, 0x0006, "Adding Seventh Entry");
    MsgTable_SetEntry(table, 0x0007, "Adding Eight Entry");
    MsgTable_SetEntry(table, 0x0008, "Adding Ninth Entry");
    MsgTable_SetEntry(table, 0x0009, "Adding Tenth Entry");
    // MsgTable_SetEntry(table, 0x0000, "Adding Zeroth Entry");

    // MsgTable_BubbleSort(table);
    // MsgTable_QuickSort(table);

    for (u32 i = 0; i < table->count; i++) {
        recomp_printf("Position %d, Content: %s\n", table->entries[i].textId, table->entries[i].buf.schar);
    }

    MsgBuffer* entry = MsgTable_GetEntry(table, 0x0000);
    if (entry != NULL) {
        recomp_printf("LOOKUP - Content: %s\n", entry->schar);
    } else {
        recomp_printf("LOOKUP - NULL\n");
    }
} 