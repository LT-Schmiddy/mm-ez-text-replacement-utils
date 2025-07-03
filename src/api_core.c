#include "api_core.h"
#include "msg_table_custom_handles.h"
MsgTable* ETZR_mainTable;

RECOMP_DECLARE_EVENT(EZTR_OnInit());
RECOMP_DECLARE_EVENT(EZTR_OnDumpBuffer(const char* category, u16 textId, s32 len, MsgBuffer* buf));
RECOMP_DECLARE_EVENT(EZTR_OnDumpBufferFull(const char* category, u16 textId, s32 len, MsgBuffer* buf));

RECOMP_CALLBACK("*", recomp_on_init) void on_startup () {
    LOGD("Starting CustomMsgHandle_NextMessageId=0x%04X", CustomMsgHandle_NextMessageId);
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();
    ETZR_mainTable->locked = true;

    LOGI("EZTR Initialization Complete.");
}

RECOMP_EXPORT void _EZTR_ReportFatalMessage(char* msg) {
    LOGF("Dependent mod reported an fatal error: %s", msg);
}

RECOMP_EXPORT void _EZTR_ReportErrorMessage(char* msg) {
    LOGE("Dependent mod reported an error: %s", msg);
}

RECOMP_EXPORT void _EZTR_ReportWarningMessage(char* msg) {
    LOGW("Dependent mod reported an debug message: %s", msg);
}

RECOMP_EXPORT void _EZTR_ReportInfoMessage(char* msg) {
    LOGI("Dependent mod reported an info message: %s", msg);
}

RECOMP_EXPORT void _EZTR_ReportDebugMessage(char* msg) {
    LOGD("Dependent mod reported an debug message: %s", msg);
}

RECOMP_EXPORT void _EZTR_ReportVerboseMessage(char* msg) {
    LOGV("Dependent mod reported an verbose message: %s", msg);
}


// Compatability patch for an old typo:
RECOMP_EXPORT void _EXTR_ReportErrorMessage(char* msg) {
    _EZTR_ReportErrorMessage(msg);
}