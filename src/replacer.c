#include "mod_globals.h"
#include "msg_table.h"
#include "msg_table_quicksort.h"

MsgTable* table = NULL;

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    table = MsgTable_Create();
    table->automaticSorting = false;

    MsgTable_AddEntry(table, 0x0005, "Hello First Entry");
    MsgTable_AddEntry(table, 0x0004, "Adding Second Entry");
    MsgTable_AddEntry(table, 0x0003, "Adding Third Entry");
    MsgTable_AddEntry(table, 0x0002, "Adding Fourth Entry");
    MsgTable_AddEntry(table, 0x0001, "Adding Fifth Entry");
    MsgTable_AddEntry(table, 0x0000, "Adding Sixth Entry");

    // MsgTable_BubbleSort(table);
    MsgTable_QuickSort(table);

    for (u32 i = 0; i < table->count; i++) {
        recomp_printf("Position %d, Content: %s\n", table->entries[i].textId, table->entries[i].buf.schar);
    }
} 