#include "api_basic.h"
#include "msg_buffer_printf_utils.h"
#include "global.h"
#include "msg_table_custom_handles.h"

#define BASIC_API_LOCK_CHECK(table) \
if (table->locked){  \
    LOGE("Cannot modify ETZR_mainTable outside of EZTR_ON_INIT event.\n"); \
    return; \
} \

#define BASIC_API_REPLACEMENT_ID_CHECK(table, id) \
if (MsgTable_GetEntry(table, id)){  \
    LOGD("ETZR_mainTable entry 0x%04x was already assigned. Skipping.\n"); \
    return; \
} \
if (id > MSG_HIGHEST_ID) { \
    LOGE("0x%04X does not correspond to a vanilla message. No entry will be created. " \
        "To create a new message entry, use `EZTR_Basic_AddCustomText`.", textId); \
    return; \
} \

#define REPLACE_CUSTOM_DEPRECATED_WARNING(ALT_func) \
LOGW("The function %s is deprecated, and will be removed in a future version of EZTR. Use %s instead.", __func__, #ALT_func); 

// Replacing Vanilla Text:
RECOMP_EXPORT void EZTR_Basic_ReplaceBuffer(u16 textId, MsgBuffer* buf, MsgCallback callback) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);
    BASIC_API_REPLACEMENT_ID_CHECK(ETZR_mainTable, textId);

    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf, callback);
    LOGI("Buffer set for message 0x%04X", (u32)textId);
}


RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback, ...) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);
    BASIC_API_REPLACEMENT_ID_CHECK(ETZR_mainTable, textId);

    MsgBuffer* buf = MsgBuffer_Create();
    
    MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

    va_list va;
    pf_va_start(va, callback);
    const int ret = _MsgSContent_Vsnprintf(_out_buffer, pipe_escape_bytes, buf->data.content, MSG_CONTENT_SIZE, content, (size_t)-1, va);
    pf_va_end(va);
    
    
    MsgTable_StoreBuffer(ETZR_mainTable, textId, buf, callback);
    LOGI("Buffer set for message 0x%04X", (u32)textId);

    MsgBuffer_Destroy(buf);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceTextEmpty(u16 textId, MsgCallback callback) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);
    BASIC_API_REPLACEMENT_ID_CHECK(ETZR_mainTable, textId);
    
    MsgBuffer* buf = MsgBuffer_Create();

    MsgSContent_Sprintf((char*)&buf->data.content, "\xBF");

    MsgTable_StoreBufferEmpty(ETZR_mainTable, textId, callback);
    LOGI("Empty Buffer set for message 0x%04X", (u32)textId);
    MsgBuffer_Destroy(buf);
}

// Adding Custom Buffers:
RECOMP_EXPORT void EZTR_Basic_AddCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);

    u16 new_id = CustomMsgHandle_NextMessageId;
    // Setting the handle ID:
    bool success = CustomMsgHandle_AssignAndValidate(handle);

    if (success) {
        MsgTable_StoreBuffer(ETZR_mainTable, new_id, buf, callback);
        LOGI("Buffer added for custom message 0x%04X", new_id);
    } else {
        LOGD("Buffer for custom message 0x%04X was not added", new_id);
    }
}

RECOMP_EXPORT void EZTR_Basic_AddCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback, ...) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);
    
    u16 new_id = CustomMsgHandle_NextMessageId;
    // Setting the handle ID:
    bool success = CustomMsgHandle_AssignAndValidate(handle);

    if (success) {
        MsgBuffer* buf = MsgBuffer_Create();
        MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, second_item_rupees);

        va_list va;
        pf_va_start(va, callback);
        const int ret = _MsgSContent_Vsnprintf(_out_buffer, pipe_escape_bytes, buf->data.content, MSG_CONTENT_SIZE, content, (size_t)-1, va);
        pf_va_end(va);

        MsgTable_StoreBuffer(ETZR_mainTable, new_id, buf, callback);
        MsgBuffer_Destroy(buf);
        LOGI("Buffer added for custom message 0x%04X", new_id);
    } else {
        LOGD("Buffer for custom message 0x%04X was not added", new_id);
    }
}

RECOMP_EXPORT void EZTR_Basic_AddCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback) {
    BASIC_API_LOCK_CHECK(ETZR_mainTable);

    u16 new_id = CustomMsgHandle_NextMessageId;
    // Setting the handle ID:
    bool success = CustomMsgHandle_AssignAndValidate(handle);

    if (success) {
        MsgTable_StoreBufferEmpty(ETZR_mainTable, new_id, callback);
        LOGI("Empty buffer added for custom message 0x%04X", new_id);
    } else {
        LOGD("Buffer for custom message 0x%04X was not added",new_id);
    }

}

// DEPRECATED - Replacing Custom Messages:
RECOMP_EXPORT void EZTR_Basic_ReplaceCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback) {
    REPLACE_CUSTOM_DEPRECATED_WARNING(EZTR_Basic_AddCustomBuffer);

    EZTR_Basic_AddCustomBuffer(handle, buf, callback);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback, ...) {
    REPLACE_CUSTOM_DEPRECATED_WARNING(EZTR_Basic_AddCustomText);

    EZTR_Basic_AddCustomText(handle, text_box_type, text_box_y_pos, display_icon, next_message_id, first_item_rupees, 
        second_item_rupees, pipe_escape_bytes, content, callback);
}

RECOMP_EXPORT void EZTR_Basic_ReplaceCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback) {
    REPLACE_CUSTOM_DEPRECATED_WARNING(EZTR_Basic_AddCustomTextEmpty);

    EZTR_Basic_AddCustomTextEmpty(handle, callback);
}