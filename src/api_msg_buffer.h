#ifndef __API_MSG_BUFFER__
#define __API_MSG_BUFFER__

#include "msg_buffer.h"
#include "msg_buffer_printf.h"

// Lifetime:

/**
 * @brief Create msgBuffer
 * 
 * @return MsgBuffer* 
 */
MsgBuffer* EZTR_MsgBuffer_Create();
MsgBuffer* EZTR_MsgBuffer_CreateFromStr(char* src);
MsgBuffer* EZTR_MsgBuffer_CreateFromStrN(char* src, size_t len);
void EZTR_MsgBuffer_Destroy(MsgBuffer* buf);

// Copy:

u32 EZTR_MsgBuffer_Copy(MsgBuffer* dst, char* src);
u32 EZTR_MsgBuffer_NCopy(MsgBuffer* dst, char* src, size_t len);
u32 EZTR_MsgBuffer_Len(MsgBuffer* buf);
u32 EZTR_MsgBuffer_ContentLen(MsgBuffer* buf);
void EZTR_MsgBuffer_WriteDefaultHeader(MsgBuffer* buf);
void EZTR_MsgBuffer_WriteHeader(MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees);

u8 EZTR_MsgBuffer_GetTextBoxType(MsgBuffer* buf);
void EZTR_MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type);
u8 EZTR_MsgBuffer_GetTextBoxYPos(MsgBuffer* buf);
void EZTR_MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos);
u8 EZTR_MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf);
void EZTR_MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon);
u16 EZTR_MsgBuffer_GetNextMsg(MsgBuffer* buf);
void EZTR_MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 type);
u16 EZTR_MsgBuffer_GetFirstItemRupees(MsgBuffer* buf);
void EZTR_MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 pos);
u16 EZTR_MsgBuffer_GetSecondItemRupees(MsgBuffer* buf);
void EZTR_MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 pos);
char* EZTR_MsgBuffer_GetContentPtr(MsgBuffer* buf);

void EZTR_MsgSContent_SetEmpty(char* cont);
u32 EZTR_MsgSContent_Len(char* cont);
u32 EZTR_MsgSContent_Copy(char* dst, char* src);
u32 EZTR_MsgSContent_NCopy(char* dst, char* src, size_t len);
char* EZTR_MsgSContent_NCat(char* dst, char* src, size_t len);
char* EZTR_MsgSContent_Cat(char* dst, char* src);
s32 EZTR_MsgSContent_NCmp(char* str1, char* str2, size_t len);
s32 EZTR_MsgSContent_Cmp(char* str1, char* str2);


int EZTR_MsgSContent_Printf(const char* format, ...);
int EZTR_MsgSContent_Sprintf(char* buffer, const char* format, ...);
int EZTR_MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...);
int EZTR_MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va);
int EZTR_MsgSContent_Vprintf(const char* format, va_list va);
int EZTR_MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);

#endif