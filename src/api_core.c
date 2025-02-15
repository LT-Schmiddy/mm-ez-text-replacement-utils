#include "api_core.h"

MsgTable* ETZR_mainTable;

RECOMP_DECLARE_EVENT(EZTR_OnInit());

RECOMP_CALLBACK("*", recomp_on_init) void on_startup () {
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();

    recomp_printf("EZTR Initialization Complete.\n");
}
