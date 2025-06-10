#include "api_core.h"

MsgTable* ETZR_mainTable;

RECOMP_DECLARE_EVENT(EZTR_OnInit());

RECOMP_CALLBACK("*", recomp_on_init) void on_startup () {
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();
    ETZR_mainTable->locked = true;

    LOGI("EZTR Initialization Complete.");
}


RECOMP_EXPORT void _EXTR_ReportErrorMessage(char* error_msg) {
    LOGE("Dependent mod reported an error: %s", error_msg);
}