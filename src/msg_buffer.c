#include "msg_buffer.h"

MsgBuffer* MsgBuffer_Create() {
    return recomp_alloc(sizeof(MsgBuffer));
}

void MsgBuffer_Destroy(MsgBuffer* buf) {
    recomp_free(buf);
}

u32 MsgBuffer_StrCopy(char* dst, char* src) {
    int i = 0;
    for (; (dst[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < MSG_BUFFER_SIZE; i++) {
        dst[i] = src[i];
    }
    if (i < MSG_BUFFER_SIZE - 1) {
        dst[i+1] = '\xBF';
    }  else {
        // Otherwise at the max index
        dst[i] = '\xBF';
    }
    return i;
}

MsgBuffer* MsgBuffer_Load(char* src) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_StrCopy((char*)buf, src);
    return buf;
}
char* MsgBuffer_Store(MsgBuffer* buf) {
    u32 store_len = MsgBuffer_Len(buf) + 1;
    char* retVal = recomp_alloc(store_len);
    MsgBuffer_StrCopy(retVal, (char*)buf);
    return retVal;
}

u32 MsgBuffer_Len(MsgBuffer* buf) {
    int i = 0;
    while ((buf->schar[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < MSG_BUFFER_SIZE) { 
        i++;
    }
    return i;
}

u32 MsgBuffer_ContentLen(MsgBuffer* buf) {
    return MsgSContent_Len(MsgBuffer_GetContentPtr(buf));
}

u32 MsgBuffer_WriteFromStr(MsgBuffer* dst, char* src) {
    IF_DEBUG recomp_printf("Copying: ");
    u32 i = 0;

    bool should_end = false;
    while (!should_end && i < MSG_CONTENT_SIZE) {
        IF_DEBUG recomp_printf( is_printable_char(src[i]) ? "%c" : "\\x%02X", src[i]);
        dst->schar[i + MSG_HEADER_SIZE] = src[i];
        i++;
        should_end = src[i] == MSG_ENDING_CHAR;
    }
    
    IF_DEBUG recomp_printf(" -> %i\n", i); 
    // Add 1 to go from index to length. 
    return i + 1;
}

void MsgBuffer_WriteDefaultHeader(MsgBuffer* buf) {
    MsgBuffer_SetTextBoxType(buf, EZTR_STANDARD_TEXT_BOX_I);
    MsgBuffer_SetTextBoxYPos(buf, 1);
    MsgBuffer_SetTextBoxDisplayIcon(buf, EZTR_ICON_NO_ICON);
    MsgBuffer_SetNextMsg(buf, EZTR_NO_VALUE);
    MsgBuffer_SetFirstItemRupees(buf, EZTR_NO_VALUE);
    MsgBuffer_SetSecondItemRupees(buf, EZTR_NO_VALUE);
}

void MsgBuffer_WriteHeader(MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees) {
    MsgBuffer_SetTextBoxType(buf, text_box_type);
    MsgBuffer_SetTextBoxYPos(buf, text_box_y_pos);
    MsgBuffer_SetTextBoxDisplayIcon(buf, display_icon);
    MsgBuffer_SetNextMsg(buf, next_message_id);
    MsgBuffer_SetFirstItemRupees(buf, first_item_rupees);
    MsgBuffer_SetSecondItemRupees(buf, second_item_rupees);
}

// Getters/Setters for the header:
u8 MsgBuffer_GetTextBoxType(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[0], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type) {
    memcpy(&buf->schar[0], &type, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxYPos(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[1], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos) {
    memcpy(&buf->schar[1], &pos, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->schar[2], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon) {
    memcpy(&buf->schar[2], &icon, sizeof(u8));
}


u16 MsgBuffer_GetNextMsg(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[3], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 nextId) {
    memcpy(&buf->schar[3], &nextId, sizeof(u16));
}


u16 MsgBuffer_GetFirstItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[5], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 value) {
    memcpy(&buf->schar[5], &value, sizeof(u16));
}


u16 MsgBuffer_GetSecondItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->schar[7], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 value) {
    memcpy(&buf->schar[7], &value, sizeof(u16));
}

// Content Stuff
MsgSContent* MsgBuffer_GetContentPtr(MsgBuffer* buf) {
   return (MsgSContent*)MsgBuffer_GetContentCharPtr(buf);
}

char* MsgBuffer_GetContentCharPtr(MsgBuffer* buf) {
   return &buf->schar[MSG_HEADER_SIZE];
}

void MsgSContent_SetEmpty(MsgSContent* cont) {
    char* c = (char*)cont;
    for (int i = 0; i < MSG_CONTENT_SIZE; i++) {
        c[i] = MSG_ENDING_CHAR;
    }
}

u32 MsgSContent_Len(MsgSContent* cont) {
    int i = 0;
    char* c = (char*)cont;
    while (c[i] != MSG_ENDING_CHAR && i < MSG_CONTENT_SIZE) {
        i++;
    }

    return i;
}