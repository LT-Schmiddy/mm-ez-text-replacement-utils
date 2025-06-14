#ifndef __MSG_BUFFER__
#define __MSG_BUFFER__

#include "mod_globals.h"
#include "libc/string.h"
#include "util.h"
#include "api_enums.h"
#include "msg_buffer_printf.h"

extern const char* msg_control_code_names[];
extern const char* eztr_textbox_names[];
extern const char* eztr_icon_names[];

typedef union {
    char schar[MSG_BUFFER_SIZE]; // msgBuf
    u16 wchar[MSG_BUFFER_WIDE_SIZE];   // msgBufWide
    u64 force_structure_alignment_msg;
} MsgRaw;

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
        MsgRaw raw;
        MsgData data;
        MsgPartition partitions;
} MsgBuffer;


MsgBuffer* MsgBuffer_Create();
void MsgBuffer_Destroy(MsgBuffer* buf);
u32 MsgBuffer_Copy(MsgBuffer* dst, char* src);
u32 MsgBuffer_NCopy(MsgBuffer* dst, char* src, size_t len);

MsgBuffer* MsgBuffer_CreateFromStr(char* src);
MsgBuffer* MsgBuffer_CreateFromStrN(char* src, size_t len);
char* MsgBuffer_ShrinkForStorage(MsgBuffer* buf);

u32 MsgBuffer_Len(MsgBuffer* buf);
u32 MsgBuffer_ContentLen(MsgBuffer* buf);
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
void MsgBuffer_Print(MsgBuffer* buf);
void MsgBuffer_PrintCCode(u16 textid, MsgBuffer* buf);
void MsgBuffer_PrintFull(MsgBuffer* buf);
void MsgBuffer_PrintFullCCode(u16 textId, MsgBuffer* buf);
char* MsgBuffer_GetContentPtr(MsgBuffer* buf);
// Msg Content:

void MsgSContent_SetEmpty(char* cont);
u32 MsgSContent_Len(char* cont);
u32 MsgSContent_NCopy(char* dst, char* src, size_t len);
u32 MsgSContent_Copy(char* dst, char* src);
char* MsgSContent_NCat(char* dst, char* src, size_t len);
char* MsgSContent_Cat(char* dst, char* src);
s32 MsgSContent_NCmp(char* str1, char* str2, size_t len);
s32 MsgSContent_Cmp(char* str1, char* str2);

#endif