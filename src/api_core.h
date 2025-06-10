#include "mod_globals.h"
#include "msg_table.h"
#include "api_enums.h"

extern MsgTable* ETZR_mainTable;
void _EXTR_ReportErrorMessage(char* error_msg);

void EZTR_OnDumpBuffer(const char* category, u16 textId, s32 len, MsgBuffer* buf);
void EZTR_OnDumpBufferFull(const char* category, u16 textId, s32 len, MsgBuffer* buf);
