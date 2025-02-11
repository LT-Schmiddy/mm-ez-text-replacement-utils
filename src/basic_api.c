#include "basic_api.h"

MsgTable* ETZR_mainTable;

RECOMP_DECLARE_EVENT(EZTR_OnInit());

/* RECOMP_EXPORT */ void EZTR_Basic_ReplaceText_Callback(u16 textId,
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees,
    bool pipe_escape_bytes,
    char* content,
    MsgCallback callback
) {
    MsgBuffer buf;
    memcpy(&buf.schar[0], &text_box_type, sizeof(u8));
    memcpy(&buf.schar[1], &text_box_y_pos, sizeof(u8));
    memcpy(&buf.schar[2], &display_icon, sizeof(u8));
    memcpy(&buf.schar[3], &next_message_id, sizeof(u16));
    memcpy(&buf.schar[5], &first_item_rupees, sizeof(u16));
    memcpy(&buf.schar[7], &second_item_rupees, sizeof(u16));

    if (pipe_escape_bytes) {
        
        MsgBuffer_WriteFromStr_PipeEscapeBytes(&buf, content);
    } else {
        MsgBuffer_WriteFromStr(&buf, content);
    }

    MsgTable_SetBuffer(ETZR_mainTable, textId, &buf);
    MsgTable_SetCallback(ETZR_mainTable, textId, callback);
    recomp_printf("Buffer Added 0x%04X\n", (u32)textId);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId,
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees,
    bool pipe_escape_bytes,
    char* content
) {
    EZTR_Basic_ReplaceText_Callback(textId, text_box_type, text_box_y_pos, display_icon, 
        next_message_id, first_item_rupees, second_item_rupees, pipe_escape_bytes, content, NULL);
}

void test_callback(PlayState* play, u16 textId, MsgBuffer* buf) {
    recomp_printf("Running Test Callback:\n");
    MsgSContent* content = MsgBuffer_GetContentPtr(buf);
    static int i = 0;
    i++;
    MsgSContent_SprintfChar(content, "You've read this sign %i times.\xBF", i);
    MsgSContent_Printf(content);

}

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();

    // MsgBuffer* buf = MsgTable_GetBuffer(ETZR_mainTable, 0x1785);
    // s32 len = MsgBuffer_Len(buf);
    // s32 clen = MsgBuffer_ContentLen(buf);
    // s32 diff = len - clen;
    // recomp_printf("TEST: Len = %i, Content Len = %i, Diff = %i\n", len, clen, diff);

    // MsgSContent_Printf("%m|BF", &c);

    EZTR_Basic_ReplaceText_Callback(0x0314, EZTR_WOODEN_SIGN_BACKGROUND, 32, EZTR_ICON_NO_ICON, EZTR_NO_VALUE, EZTR_NO_VALUE, 
        EZTR_NO_VALUE, true, "|01This way to Snowhead.|11Beware of the slippery valley trail|11and the giant falling snowballs|11", test_callback);

}
