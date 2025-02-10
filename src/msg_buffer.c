#include "msg_buffer.h"

s32 MsgBuffer_Len(MsgBuffer* buf) {

    IF_DEBUG recomp_printf("Counting: ");
    int i = 0;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MESSAGE_BUFFER_SIZE) {
        IF_DEBUG recomp_printf( is_printable_char(buf->schar[i]) ? "%c" : "\\x%1X", buf->schar[i]);    
        i++;
    }

    IF_DEBUG recomp_printf(" -> %i\n", i + 2);  
    // Add 2 to include termination char, and to go from index to length.
    return i + 2;
}

s32 MsgBuffer_CopyFromCharStr(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying: ");
    int i = 0;

    bool should_end = false;
    while (!should_end && i < MESSAGE_CONTENT_SIZE) {

        IF_DEBUG recomp_printf( is_printable_char(src[i]) ? "%c" : "\\x%02X", src[i]);
        dst->schar[i + MESSAGE_HEADER_SIZE] = src[i];
        i++;

        should_end = src[i] == MSG_ENDING_CHAR;
    }
    
    IF_DEBUG recomp_printf(" -> %i\n", i); 
    // Add 1 to go from index to length. 
    return i + 1;
}

s32 MsgBuffer_CopyFromCharStr_PipeEscapeBytes(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying w/ Pipes: ");
    int src_pos = 0;
    int dst_pos = 0;
    bool should_end = false;
    while (!should_end && dst_pos < MESSAGE_CONTENT_SIZE) {

        if (src[src_pos] != PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
            IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
            should_end = src[src_pos] == MSG_ENDING_CHAR;
            src_pos++;
            dst_pos++;
        } else if (src[src_pos + 1] == PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = PIPE_CHAR;
            src_pos += 2;
            dst_pos++;
        } else {
            // Parsing Byte Escape:
            IF_DEBUG recomp_printf("%c%c", src[src_pos + 1], src[src_pos + 2]);
            char write_byte = hex_to_byte(&src[src_pos + 1]);
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = write_byte;
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