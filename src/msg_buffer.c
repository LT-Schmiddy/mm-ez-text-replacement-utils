#include "msg_buffer.h"

s32 MsgBuffer_Len(MsgBuffer* buf) {
    int i = 0;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MSG_BUFFER_SIZE) { 
        i++;
    }

    return i;
}

s32 MsgBuffer_ContentLen(MsgBuffer* buf) {
    int i = MSG_HEADER_SIZE;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MSG_BUFFER_SIZE) {
        i++;
    }

    return i;
}

s32 MsgBuffer_WriteFromStr(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying: ");
    int i = 0;

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

s32 MsgBuffer_WriteFromStr_PipeEscapeBytes(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying w/ Pipes: ");
    int src_pos = 0;
    int dst_pos = 0;
    bool should_end = false;
    while (!should_end && dst_pos < MSG_CONTENT_SIZE) {

        if (src[src_pos] != PIPE_CHAR) {
            dst->schar[dst_pos + MSG_HEADER_SIZE] = src[src_pos];
            IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
            should_end = src[src_pos] == MSG_ENDING_CHAR;
            src_pos++;
            dst_pos++;
        } else if (src[src_pos + 1] == PIPE_CHAR) {
            dst->schar[dst_pos + MSG_HEADER_SIZE] = PIPE_CHAR;
            src_pos += 2;
            dst_pos++;
        } else {
            // Parsing Byte Escape:
            IF_DEBUG recomp_printf("|%c%c", src[src_pos + 1], src[src_pos + 2]);
            char write_byte = hex_to_byte(&src[src_pos + 1]);
            dst->schar[dst_pos + MSG_HEADER_SIZE] = write_byte;
            should_end = write_byte == MSG_ENDING_CHAR;
            src_pos += 3;
            dst_pos++;
        }

    }
    // Copy Final Char:
    IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
    // dst->schar[src_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
    
    IF_DEBUG recomp_printf(" -> %i\n", src_pos); 
    // Add 1 to go from index to length. 
    return dst_pos + 1;
}

void MsgBuffer_WriteDefaultHeader(MsgBuffer* buf) {
    MsgBuffer_SetTextBoxType(buf, EZTR_STANDARD_TEXT_BOX_I);
    MsgBuffer_SetTextBoxYPos(buf, 1);
    MsgBuffer_SetTextBoxDisplayIcon(buf, EZTR_ICON_NO_ICON);
    MsgBuffer_SetNextMsg(buf, EZTR_NO_VALUE);
    MsgBuffer_SetFirstItemRupees(buf, EZTR_NO_VALUE);
    MsgBuffer_SetSecondItemRupees(buf, EZTR_NO_VALUE);
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


MsgBufferSContent* MsgBuffer_GetContentPtr(MsgBuffer* buf) {
   return (MsgBufferSContent*)&buf->schar[MSG_HEADER_SIZE];
}

void MsgBufferSContent_SetEmpty(MsgBufferSContent* cont) {
    for (int i = 0; i < MSG_CONTENT_SIZE; i++) {
        (*cont[i]) = '\xBF';
    }
}