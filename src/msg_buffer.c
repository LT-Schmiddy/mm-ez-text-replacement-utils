#include "msg_buffer.h"

MsgBuffer* MsgBuffer_Create() {
    return recomp_alloc(sizeof(MsgBuffer));
}

void MsgBuffer_Destroy(MsgBuffer* buf) {
    recomp_free(buf);
}

MsgBuffer* MsgBuffer_CreateFromStr(char* src) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_Copy(buf, src);
    return buf;
}

MsgBuffer* MsgBuffer_CreateFromStrN(char* src, size_t len) {
    MsgBuffer* buf = MsgBuffer_Create();
    MsgBuffer_NCopy(buf, src, len);
    return buf;
}

u32 MsgBuffer_Copy(MsgBuffer* dst, char* src) {
    return MsgBuffer_NCopy(dst, src, MSG_BUFFER_SIZE);
}
u32 MsgBuffer_NCopy(MsgBuffer* dst, char* src, size_t len) {
    IF_DEBUG recomp_printf("StrNCopying Message Data: %p -> %p ", src, dst);

    u32 i = 0;
    for (; (src[i] != MSG_ENDING_CHAR || i < MSG_HEADER_SIZE) && i < len; i++) {
        dst->raw.schar[i] = src[i];
    }
    if (i < len) {
        dst->raw.schar[i] = '\xBF';
    }  else {
        // Otherwise at the max index
        dst->raw.schar[i-1] = '\xBF';
    }
    IF_DEBUG MsgSContent_Printf("%m\xBF", dst->data.content);
    IF_DEBUG recomp_printf("\n");
    return i;
}

char* MsgBuffer_ShrinkForStorage(MsgBuffer* buf) {
    size_t store_len = MsgBuffer_Len(buf);
    // The extra byte is to store the \xBF:
    recomp_printf("Storage Size: %u\n", store_len);
    char* retVal = recomp_alloc(sizeof(char) * store_len);
    
    // Not really meant for this, but it's fine.
    u32 copy_len = MsgBuffer_NCopy((MsgBuffer*)retVal, (char*)buf, store_len);
    if (copy_len != store_len) {
        recomp_printf("WARNING: STORAGE SIZE MISMATCH! Storage: %u, Length: %u\n", store_len, copy_len);
    };
    return retVal;
}

u32 MsgBuffer_Len(MsgBuffer* buf) {
    u32 i = 0;
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
    *c = '\xBF';
    // for (int i = 0; i < MSG_CONTENT_SIZE; i++) {
    //     c[i] = MSG_ENDING_CHAR;
    // }
}


// MsgSConetent Functions - Parallel to C String Functions.
u32 MsgSContent_Len(char* cont) {
    u32 i = 0;
    char* c = (char*)cont;
    while (c[i] != MSG_ENDING_CHAR && i < MSG_CONTENT_SIZE) {
        i++;
    }
    return i;
}

u32 MsgSContent_NCopy(char* dst, char* src, size_t len) {
    IF_DEBUG recomp_printf("StrNCopying Message Content: %p -> %p ", src, dst);

    u32 i = 0;
    for (; (src[i] != MSG_ENDING_CHAR) && i < len; i++) {
        dst[i] = src[i];
    }
    if (i < len) {
        dst[i] = '\xBF';
    }  else {
        // Otherwise at the max index
        dst[i-1] = '\xBF';
    }
    IF_DEBUG MsgSContent_Printf("%m\xBF", dst);
    IF_DEBUG recomp_printf("\n");
    return i;
}

u32 MsgSContent_Copy(char* dst, char* src) {
    return MsgSContent_NCopy(dst, src, MSG_CONTENT_SIZE);
}

char* MsgSContent_NCat(char* dst, char* src, size_t len) {
    u32 slen = MsgSContent_Len(dst);
    u32 copy_size = MSG_CONTENT_SIZE - slen;
    MsgSContent_NCopy(&dst[slen], src, MIN(copy_size, len));

    return dst;
}

char* MsgSContent_Cat(char* dst, char* src) {
    return MsgSContent_NCat(dst, src, MSG_CONTENT_SIZE);
}

s32 MsgSContent_NCmp(char* str1, char* str2, size_t len) {
    size_t slen1 = MIN(MsgSContent_Len(str1), len);
    size_t slen2 = MIN(MsgSContent_Len(str2), len);

    if (slen1 > slen2) {
        return 1;
    } else if ( slen1 < slen2) {
        return -1;
    }

    // Strings have equal lentgh, so slen1 and slen2 are interchangable:
    for (u32 i = 0; i < slen1; i++) {
        s32 diff = str1[i] - str2[i];
        if (diff != 0) {
            // Found a mismatch:
            return diff / ABS(diff);
        }
    }

    return 0;
}

s32 MsgSContent_Cmp(char* str1, char* str2) {
    return MsgSContent_NCmp(str1, str2, MSG_CONTENT_SIZE);
}