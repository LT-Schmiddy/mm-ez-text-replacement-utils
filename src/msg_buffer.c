#include "msg_buffer.h"

MsgBuffer* MsgBuffer_Create() {
    return recomp_alloc(sizeof(MsgBuffer));
}

void MsgBuffer_Destroy(MsgBuffer* buf) {
    recomp_free(buf);
}

u32 MsgBuffer_StrCopy(char* dst, char* src) {
    IF_DEBUG recomp_printf("StrCopying Message Data: %p -> %p : ", src, dst);
    
    int i = 0;
    for (; (src[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < MSG_BUFFER_SIZE; i++) {
        dst[i] = src[i];
    }
    if (i < MSG_BUFFER_SIZE) {
        dst[i] = '\xBF';
    }  else {
        // Otherwise at the max index
        dst[i-1] = '\xBF';
    }
    MsgSContent_Printf("%m\xBF", dst);
    recomp_printf("\n");
    return i;
}
u32 MsgBuffer_StrNCopy(char* dst, char* src, size_t len) {
    IF_DEBUG recomp_printf("StrNCopying Message Data: %p -> %p ", src, dst);

    u32 i = 0;
    for (; (src[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < len; i++) {
        dst[i] = src[i];
    }
    if (i < MSG_BUFFER_SIZE) {
        dst[i] = '\xBF';
    }  else {
        // Otherwise at the max index
        dst[i-1] = '\xBF';
    }
    MsgSContent_Printf("%m\xBF", dst);
    recomp_printf("\n");
    return i;
}

MsgBuffer* MsgBuffer_CreateFromStr(char* src) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_StrCopy((char*)buf, src);
    return buf;
}

MsgBuffer* MsgBuffer_CreateFromStrN(char* src, size_t len) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_StrNCopy(buf->raw.schar, src, len);
    return buf;
}

char* MsgBuffer_ShrinkForStorage(MsgBuffer* buf) {
    size_t store_len = MsgBuffer_Len(buf);
    // The extra byte is to store the \xBF:
    recomp_printf("Storage Size: %u\n", store_len + 1);
    char* retVal = recomp_alloc(sizeof(char) * store_len);
    // char* retVal = recomp_alloc(sizeof(MsgBuffer));
    u32 copy_len = MsgBuffer_StrCopy(retVal, (char*)buf);
    if (copy_len != store_len) {
        recomp_printf("WARNING: STORAGE SIZE MISMATCH! Storage: %u, Length: %u\n", store_len, copy_len);
    };
    return retVal;
}

u32 MsgBuffer_Len(MsgBuffer* buf) {
    int i = 0;
    while ((buf->raw.schar[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < MSG_BUFFER_SIZE) { 
        i++;
    }
    return i;
}

u32 MsgBuffer_ContentLen(MsgBuffer* buf) {
    return MsgSContent_Len(buf->data.content);
}

u32 MsgBuffer_WriteFromStr(MsgBuffer* dst, char* src) {
    IF_DEBUG recomp_printf("Copying: ");
    u32 i = 0;

    bool should_end = false;
    while (!should_end && i < MSG_CONTENT_SIZE) {
        IF_DEBUG recomp_printf( is_printable_char(src[i]) ? "%c" : "\\x%02X", src[i]);
        dst->raw.schar[i + MSG_HEADER_SIZE] = src[i];
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
    memcpy(&retVal, &buf->raw.schar[0], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type) {
    memcpy(&buf->raw.schar[0], &type, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxYPos(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->raw.schar[1], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos) {
    memcpy(&buf->raw.schar[1], &pos, sizeof(u8));
}


u8 MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf) {
    u8 retVal;
    memcpy(&retVal, &buf->raw.schar[2], sizeof(u8));
    return retVal;
}

void MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon) {
    memcpy(&buf->raw.schar[2], &icon, sizeof(u8));
}


u16 MsgBuffer_GetNextMsg(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->raw.schar[3], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 nextId) {
    memcpy(&buf->raw.schar[3], &nextId, sizeof(u16));
}


u16 MsgBuffer_GetFirstItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->raw.schar[5], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 value) {
    memcpy(&buf->raw.schar[5], &value, sizeof(u16));
}


u16 MsgBuffer_GetSecondItemRupees(MsgBuffer* buf) {
    u16 retVal;
    memcpy(&retVal, &buf->raw.schar[7], sizeof(u16));
    return retVal;
}

void MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 value) {
    memcpy(&buf->raw.schar[7], &value, sizeof(u16));
}

// Content Stuff

char* MsgBuffer_GetContentPtr(MsgBuffer* buf) {
   return &buf->raw.schar[MSG_HEADER_SIZE];
}

void MsgSContent_SetEmpty(char* cont) {
    char* c = (char*)cont;
    for (int i = 0; i < MSG_CONTENT_SIZE; i++) {
        c[i] = MSG_ENDING_CHAR;
    }
}

u32 MsgSContent_Len(char* cont) {
    int i = 0;
    char* c = (char*)cont;
    while (c[i] != MSG_ENDING_CHAR && i < MSG_CONTENT_SIZE) {
        i++;
    }

    return i;
}