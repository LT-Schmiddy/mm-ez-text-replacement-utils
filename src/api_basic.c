#include "api_basic.h"

RECOMP_DECLARE_EVENT(EZTR_OnInit());

RECOMP_EXPORT void EZTR_Basic_ReplaceText_Callback(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    MsgSContent* cont = MsgBuffer_GetContentPtr(buf);
    if (pipe_escape_bytes) {
        
        // MsgBuffer_WriteFromStr_PipeEscapeBytes(&buf, content);
        MsgSContent_SprintfChar(cont, "%m\xBF", content);
    } else {
        MsgBuffer_WriteFromStr(buf, content);
    }

    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf);
    MsgTable_SetCallback(ETZR_mainTable, textId, callback);
    recomp_printf("Buffer Added 0x%04X\n", (u32)textId);
    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content) {
    EZTR_Basic_ReplaceText_Callback(textId, text_box_type, text_box_y_pos, display_icon, 
        next_message_id, first_item_rupees, second_item_rupees, pipe_escape_bytes, content, NULL);
}

void test_callback(MsgBuffer* buf, u16 textId, PlayState* play) {
    recomp_printf("Running Test Callback:\n");
    MsgSContent* content = MsgBuffer_GetContentPtr(buf);
    // static int i = 0;
    // i++;
    // MsgSContent_SprintfChar(content, "You've read this sign %i times %m %s.|BF", i, "(A LOT)\xBF");
    MsgSContent_SprintfChar(content, "You've read this sign %m.|BF", "(A LOT)\xBF");
    // MsgSContent_Printf(content);

}

