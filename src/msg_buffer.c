#include "msg_buffer.h"
#include "util.h"

#include "recomp_logging.h"


MsgBuffer* MsgBuffer_Create() {
    MsgBuffer* retVal = recomp_alloc(sizeof(MsgBuffer));
    MsgBuffer_WriteDefaultHeader(retVal);
    MsgSContent_SetEmpty(retVal->data.content);

    return retVal;
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
    LOGV("StrNCopying Message Data: %p -> %p ", src, dst);

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
    IF_LOG_VERBOSE MsgSContent_PrintfLn("%m\xBF", dst->data.content);
    return i;
}

char* MsgBuffer_ShrinkForStorage(MsgBuffer* buf) {
    size_t store_len = MsgBuffer_Len(buf)+1;
    // The extra byte is to store the \xBF:
    LOGV("Storage Size: %u", store_len);
    char* retVal = recomp_alloc(sizeof(char) * store_len);
    
    // Not really meant for this, but it's fine.
    u32 copy_len = MsgBuffer_NCopy((MsgBuffer*)retVal, (char*)buf, store_len)+1;
    if (copy_len != store_len) {
        LOGW("WARNING: STORAGE SIZE MISMATCH! Storage: %u, Length: %u", store_len, copy_len);
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

void MsgBuffer_WriteDefaultHeader(MsgBuffer* buf) {
    MsgBuffer_SetTextBoxType(buf, EZTR_STANDARD_TEXT_BOX_I);
    MsgBuffer_SetTextBoxYPos(buf, 0);
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


void MsgBuffer_Print(MsgBuffer* buf) {
    // Loading Header Info:
    u8 text_box_type = 0;
    u8 text_box_y_pos = 0;
    u8 display_icon = 0;
    u16 next_message_id = 0xffff;
    u16 first_item_rupees = 0xffff;
    u16 second_item_rupees = 0xffff;

    memcpy(&text_box_type, &buf->raw.schar[0], sizeof(u8));
    memcpy(&text_box_y_pos, &buf->raw.schar[1], sizeof(u8));
    memcpy(&display_icon, &buf->raw.schar[2], sizeof(u8));
    memcpy(&next_message_id, &buf->raw.schar[3], sizeof(u16));
    memcpy(&first_item_rupees, &buf->raw.schar[5], sizeof(u16));
    memcpy(&second_item_rupees, &buf->raw.schar[7], sizeof(u16));

    // Printing Output:
    recomp_printf("MsgBuffer:\n");
    recomp_printf("\tTextbox Type: %s\n", eztr_textbox_names[text_box_type]);
    recomp_printf("\tTextbox Y Position: %i\n", text_box_y_pos);
    recomp_printf("\tDisplay Icon: %s\n", eztr_icon_names[display_icon]);

    if (next_message_id == 0xffff) {
        recomp_printf("\tNext Message ID: EZTR_NO_VALUE\n");
    } else {
        char next_id_str[5] = "0000";
        message_id_as_hex(next_message_id, next_id_str);
        recomp_printf("\tNext Message ID: 0x%s\n", next_id_str);
    }
    if (first_item_rupees == 0xffff) {
        recomp_printf("\tFirst Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tFirst Item Rupees: %i\n", first_item_rupees);
    }

    if (second_item_rupees == 0xffff) {
        recomp_printf("\tSecond Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tSecond Item Rupees: %i\n", second_item_rupees);
    }

    recomp_printf("\tContent: \"");
    // Printing Message Content:
    MsgSContent_Printf("%m\xBF", buf->data.content);
    recomp_printf("\"\n\n");
}

void MsgBuffer_PrintFull(MsgBuffer* buf) {
    // Loading Header Info:
    u8 text_box_type = 0;
    u8 text_box_y_pos = 0;
    u8 display_icon = 0;
    u16 next_message_id = 0xffff;
    u16 first_item_rupees = 0xffff;
    u16 second_item_rupees = 0xffff;

    memcpy(&text_box_type, &buf->raw.schar[0], sizeof(u8));
    memcpy(&text_box_y_pos, &buf->raw.schar[1], sizeof(u8));
    memcpy(&display_icon, &buf->raw.schar[2], sizeof(u8));
    memcpy(&next_message_id, &buf->raw.schar[3], sizeof(u16));
    memcpy(&first_item_rupees, &buf->raw.schar[5], sizeof(u16));
    memcpy(&second_item_rupees, &buf->raw.schar[7], sizeof(u16));

    recomp_printf("MsgBuffer (FULL):\n");
    recomp_printf("\tTextbox Type: %s\n", eztr_textbox_names[text_box_type]);
    recomp_printf("\tTextbox Y Position: %i\n", text_box_y_pos);
    recomp_printf("\tDisplay Icon: %s\n", eztr_icon_names[display_icon]);

    if (next_message_id == 0xffff) {
        recomp_printf("\tNext Message ID: EZTR_NO_VALUE\n");
    } else {
        char next_id_str[5] = "0000";
        message_id_as_hex(next_message_id, next_id_str);
        recomp_printf("\tNext Message ID: 0x%s\n", next_id_str);
    }
    if (first_item_rupees == 0xffff) {
        recomp_printf("\tFirst Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tFirst Item Rupees: %i\n", first_item_rupees);
    }

    if (second_item_rupees == 0xffff) {
        recomp_printf("\tSecond Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tSecond Item Rupees: %i\n", second_item_rupees);
    }

    recomp_printf("\tContent: \"");
    // Printing Message Content:
    for (s32 i = 0; i < MSG_BUFFER_SIZE; i++) {
        char c = buf->raw.schar[i];

        print_char(c);
    }
     recomp_printf("\"\n\n");
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
    LOGV("StrNCopying Message Content: %p -> %p", src, dst);

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
    IF_LOG_VERBOSE MsgSContent_PrintfLn("%m\xBF", dst);
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