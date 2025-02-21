#include "api_basic.h"

#include "global.h"

RECOMP_EXPORT void EZTR_Basic_ReplaceText_WithCallback(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    if (pipe_escape_bytes) {
        
        MsgSContent_Sprintf(buf->data.content, "%m\xBF", content);
    } else {
        MsgBuffer_WriteFromStr(buf, content);
    }
    ptrdiff_t u;

    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf, callback);
    IF_DEBUG recomp_printf("Buffer Added for 0x%04X\n", (u32)textId);

    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceText_EmptyWithCallback(u16 textId, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_WriteDefaultHeader(buf);
    // char* cont = MsgBuffer_GetContentPtr(buf);
    MsgSContent_Sprintf((char*)&buf->data.content, "\xBF");

    MsgTable_StoreBufferEmpty(ETZR_mainTable, textId, callback);
    IF_DEBUG recomp_printf("Empty Buffer Added for 0x%04X\n", (u32)textId);
    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content) {
    EZTR_Basic_ReplaceText_WithCallback(textId, text_box_type, text_box_y_pos, display_icon, 
        next_message_id, first_item_rupees, second_item_rupees, pipe_escape_bytes, content, NULL);
}


RECOMP_EXPORT void EZTR_Basic_CustomText_WithCallback(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    if (pipe_escape_bytes) {
        
        MsgSContent_Sprintf(buf->data.content, "%m\xBF", content);
    } else {
        MsgBuffer_WriteFromStr(buf, content);
    }
    ptrdiff_t u;

    MsgTable_StoreCustomBuffer(ETZR_mainTable, handle, buf, callback);
    IF_DEBUG recomp_printf("Buffer Added for 0x%04X\n", (u32)handle(NULL));

    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_CustomText_EmptyWithCallback(CustomMsgHandle handle, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_WriteDefaultHeader(buf);
    // char* cont = MsgBuffer_GetContentPtr(buf);
    MsgSContent_Sprintf((char*)&buf->data.content, "\xBF");

    MsgTable_StoreCustomBufferEmpty(ETZR_mainTable, handle, callback);
    IF_DEBUG recomp_printf("Empty Buffer Added for 0x%04X\n", (u32)handle(NULL));
    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_CustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content) {
    EZTR_Basic_CustomText_WithCallback(handle, text_box_type, text_box_y_pos, display_icon, 
        next_message_id, first_item_rupees, second_item_rupees, pipe_escape_bytes, content, NULL);
}