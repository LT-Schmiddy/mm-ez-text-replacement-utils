#include "dumping.h"

#include "api_core.h"

void dump_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf) {
    // Hex conversions:
    char id_str[5] ="0000";
    message_id_as_hex(textId, id_str);
    recomp_printf("\nDumping %s Message ID 0x%s (%i):\n", category, id_str, textId);

    if (recomp_get_config_u32("dump_text_as_c_code")) {
        MsgBuffer_PrintCCode(textId, buf);
    } else {
        MsgBuffer_Print(buf);
    }


    // Handle the dump buffer event
    EZTR_OnDumpBuffer(category, textId, len, buf);
}

void dump_full_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf) {

    // Hex conversions:
    char id_str[5] ="0000";
    message_id_as_hex(textId, id_str);

    // Printing Output:
    recomp_printf("\nDumping %s Message ID 0x%s (%i):\n", category, id_str, textId);
    if (recomp_get_config_u32("dump_text_as_c_code")) {
        MsgBuffer_PrintFullCCode(textId, buf);
    } else {
        MsgBuffer_PrintFull(buf);
    }

    // Handle the dump buffer event
    EZTR_OnDumpBufferFull(category, textId, len, buf);
}