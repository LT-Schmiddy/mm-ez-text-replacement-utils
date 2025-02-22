#ifndef __API__
#define __API__

#include "libc/string.h"
#include "util.h"
#include "dumping.h"
#include "api_core.h"

// Base Replacements:
void EZTR_Basic_ReplaceText_WithCallback(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback);


void EZTR_Basic_ReplaceText(u16 textId, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content);

void EZTR_Basic_ReplaceText_EmptyWithCallback(u16 textId, MsgCallback callback);

// Custom Messages:
void EZTR_Basic_AddCustomText_WithCallback(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, MsgCallback callback);

void EZTR_Basic_CustomText(CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content);

void EZTR_Basic_CustomText_EmptyWithCallback(CustomMsgHandle handle, MsgCallback callback);

#endif