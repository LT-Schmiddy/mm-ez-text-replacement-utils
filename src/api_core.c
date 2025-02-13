#include "api_core.h"

MsgTable* ETZR_mainTable;

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();

    recomp_printf("EZTR Initialization Complete.\n");
}
