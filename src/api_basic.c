#include "api_basic.h"

#include "global.h"


// Replacing Vanilla Text:
RECOMP_EXPORT void EZTR_Basic_ReplaceBuffer(u16 textId, MsgBuffer* buf, MsgCallback callback) {
    if (textId > MSG_HIGHEST_ID) {
        LOGE("0x%04X does not correspond to a vanilla message. No entry will be created. "
            "To create a new message entry, use `EZTR_Basic_AddCustomBuffer`.", textId);
        return;
    }

    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf, callback);
    LOGI("Buffer set for messaeg 0x%04X", (u32)textId);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    if (textId > MSG_HIGHEST_ID) {
        LOGE("0x%04X does not correspond to a vanilla message. No entry will be created. "
            "To create a new message entry, use `EZTR_Basic_AddCustomText`.", textId);
        return;
    }
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);


    if (pipe_escape_bytes) {
        MsgSContent_Sprintf(buf->data.content, "%m\xBF", content);
    } else {
        MsgSContent_Copy(buf->data.content, content);
    }
    ptrdiff_t u;

    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf, callback);
    LOGI("Buffer set for message 0x%04X", (u32)textId);
    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceTextEmpty(u16 textId, MsgCallback callback) {
    if (textId > MSG_HIGHEST_ID) {
        LOGE("0x%04X does not correspond to a vanilla message. No entry will be created. "
            "To create a new message entry, use `EZTR_Basic_AddCustomTextEmpty`.", textId);
        return;
    }
    MsgBuffer* buf = MsgBuffer_Create();

    MsgSContent_Sprintf((char*)&buf->data.content, "\xBF");

    MsgTable_StoreBufferEmpty(ETZR_mainTable, textId, callback);
    LOGI("Empty Buffer set for message 0x%04X", (u32)textId);
    MsgBuffer_Destroy(buf);
}


// Adding Custom Buffers:
RECOMP_EXPORT void EZTR_Basic_AddCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback) {
    if (MsgTable_StoreNewCustomBuffer(ETZR_mainTable, handle, buf, callback)) {
        LOGI("Buffer added for custom message 0x%04X", (u32)handle(NULL));
    } else {
        LOGE("Buffer for custom message 0x%04X was not added", (u32)handle(NULL));
    }
}

RECOMP_EXPORT void EZTR_Basic_AddCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    if (pipe_escape_bytes) {
        
        MsgSContent_Sprintf(buf->data.content, "%m\xBF", content);
    } else {
        MsgSContent_Copy(buf->data.content, content);
    }

    if (MsgTable_StoreNewCustomBuffer(ETZR_mainTable, handle, buf, callback)) {
        LOGI("Buffer added for custom message 0x%04X", (u32)handle(NULL));
    } else {
        LOGE("Buffer for custom message 0x%04X was not added", (u32)handle(NULL));
    }
    

    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_AddCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();

    MsgSContent_Sprintf((char*)&buf->data.content, "\xBF");

    if (MsgTable_StoreNewCustomBufferEmpty(ETZR_mainTable, handle, callback)) {
        LOGI("Empty buffer added for custom message 0x%04X", (u32)handle(NULL));
    } else {
        LOGE("Buffer for custom message 0x%04X was not added", (u32)handle(NULL));
    }
    
    MsgBuffer_Destroy(buf);
}

// Replacing Custom Messages:
RECOMP_EXPORT void EZTR_Basic_ReplaceCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback) {
    MsgTable_StoreBuffer(ETZR_mainTable, handle(NULL), buf, callback);
    LOGI("Buffer for custom message 0x%04X changed", (u32)handle(NULL));
}

RECOMP_EXPORT void EZTR_Basic_ReplaceCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback) {
    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    if (pipe_escape_bytes) {
        
        MsgSContent_Sprintf(buf->data.content, "%m\xBF", content);
    } else {
        MsgSContent_Copy(buf->data.content, content);
    }
    ptrdiff_t u;

    MsgTable_StoreBuffer(ETZR_mainTable, handle(NULL), buf, callback);
    LOGI("Buffer for custom message 0x%04X changed", (u32)handle(NULL));

    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback) {
    MsgTable_StoreBufferEmpty(ETZR_mainTable, handle(NULL), callback);
    LOGI("Buffer for custom message 0x%04X is now empty", (u32)handle(NULL));
}