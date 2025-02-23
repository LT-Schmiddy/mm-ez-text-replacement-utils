#ifndef __API__
#define __API__

#include "libc/string.h"
#include "util.h"
#include "dumping.h"
#include "api_core.h"

// Base Replacements:

// Replacing Vanilla Text:
void EZTR_Basic_ReplaceBuffer(u16 textId, MsgBuffer* buf, MsgCallback callback);
void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback);
void EZTR_Basic_ReplaceTextEmpty(u16 textId, MsgCallback callback);

// Adding Custom Buffers:
void EZTR_Basic_AddCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback);
void EZTR_Basic_AddCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback);
void EZTR_Basic_AddCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback);

// Replacing Custom Messages:
void EZTR_Basic_ReplaceCustomBuffer(CustomMsgHandle handle, MsgBuffer* buf, MsgCallback callback);
void EZTR_Basic_ReplaceCustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback);
void EZTR_Basic_ReplaceCustomTextEmpty(CustomMsgHandle handle, MsgCallback callback);

#endif