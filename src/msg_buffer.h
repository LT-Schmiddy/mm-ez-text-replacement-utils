#ifndef __MSG_BUFFER__
#define __MSG_BUFFER__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "api_enums.h"
#include "msg_buffer_printf.h"

typedef struct __attribute__((packed)) {
    u8 text_box_type; 
    u8 text_box_y_pos;
    u8 display_icon; 
    u16 next_message_id;
    u16 first_item_rupees;
    u16 second_item_rupees;
    u16 padding;
    char content[MSG_CONTENT_SIZE];
} MsgData;

typedef struct {
    char header[MSG_HEADER_SIZE];
    char content[MSG_CONTENT_SIZE];
} MsgPartition;

typedef union {
        char schar[1280]; // msgBuf
        u16 wchar[640];   // msgBufWide
        u64 force_structure_alignment_msg;
        MsgData data;
        MsgPartition partitions;
} MsgBuffer;

MsgBuffer* MsgBuffer_Create();
void MsgBuffer_Destroy(MsgBuffer* buf);
u32 MsgBuffer_StrCopy(char* dst, char* src);
u32 MsgBuffer_StrNCopy(char* dst, char* src, size_t len);

MsgBuffer* MsgBuffer_Load(char* src);
MsgBuffer* MsgBuffer_LoadN(char* src, size_t len);
char* MsgBuffer_ShrinkForStorage(MsgBuffer* buf);

u32 MsgBuffer_Len(MsgBuffer* buf);
u32 MsgBuffer_ContentLen(MsgBuffer* buf);
u32 MsgBuffer_WriteFromStr(MsgBuffer* dst, char* src);
void MsgBuffer_WriteDefaultHeader(MsgBuffer* buf);
void MsgBuffer_WriteHeader(MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees);

// Getters/Setters for the header:
u8 MsgBuffer_GetTextBoxType(MsgBuffer* buf);
void MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type);
u8 MsgBuffer_GetTextBoxYPos(MsgBuffer* buf);
void MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos);
u8 MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf);
void MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon);
u16 MsgBuffer_GetNextMsg(MsgBuffer* buf);
void MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 type);
u16 MsgBuffer_GetFirstItemRupees(MsgBuffer* buf);
void MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 pos);
u16 MsgBuffer_GetSecondItemRupees(MsgBuffer* buf);
void MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 pos);

// Msg Content:
// char* MsgBuffer_GetContentPtr(MsgBuffer* buf);
void MsgSContent_SetEmpty(char* cont);
u32 MsgSContent_Len(char* cont);

#endif