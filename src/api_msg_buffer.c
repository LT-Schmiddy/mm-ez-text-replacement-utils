#include "api_msg_buffer.h"
#include "modding.h"


// Lifetime:
RECOMP_EXPORT MsgBuffer* EZTR_MsgBuffer_Create() {
    return MsgBuffer_Create();
}
RECOMP_EXPORT MsgBuffer* EZTR_MsgBuffer_CreateFromStr(char* src) {
    return MsgBuffer_CreateFromStr(src);
}
RECOMP_EXPORT MsgBuffer* EZTR_MsgBuffer_CreateFromStrN(char* src, size_t len) {
    return MsgBuffer_CreateFromStrN(src, len);
}

/**
 * @brief please export
 * 
 * @param buf 
 * @return RECOMP_EXPORT 
 */
RECOMP_EXPORT void EZTR_MsgBuffer_Destroy(MsgBuffer* buf) {
    MsgBuffer_Destroy(buf);
}

// Copy:
RECOMP_EXPORT u32 EZTR_MsgBuffer_Copy(MsgBuffer* dst, char* src) {
    return MsgBuffer_Copy(dst, src);
}
RECOMP_EXPORT u32 EZTR_MsgBuffer_NCopy(MsgBuffer* dst, char* src, size_t len) {
    return MsgBuffer_NCopy(dst, src, len);
}
RECOMP_EXPORT u32 EZTR_MsgBuffer_Len(MsgBuffer* buf) {
    return MsgBuffer_Len(buf);
}
RECOMP_EXPORT u32 EZTR_MsgBuffer_ContentLen(MsgBuffer* buf) {
    return MsgBuffer_ContentLen(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_WriteDefaultHeader(MsgBuffer* buf) {
    MsgBuffer_WriteDefaultHeader(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_WriteHeader(MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees) {
        MsgBuffer_WriteHeader(buf, text_box_type, text_box_y_pos, display_icon, 
            next_message_id, first_item_rupees, second_item_rupees);
}

RECOMP_EXPORT u8 EZTR_MsgBuffer_GetTextBoxType(MsgBuffer* buf) {
    return MsgBuffer_GetTextBoxType(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetTextBoxType(MsgBuffer* buf, u8 type) {
    MsgBuffer_SetTextBoxType(buf, type);
}
RECOMP_EXPORT u8 EZTR_MsgBuffer_GetTextBoxYPos(MsgBuffer* buf) {
    return MsgBuffer_GetTextBoxYPos(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetTextBoxYPos(MsgBuffer* buf, u8 pos) {
    MsgBuffer_SetTextBoxYPos(buf, pos);
}
RECOMP_EXPORT u8 EZTR_MsgBuffer_GetTextBoxDisplayIcon(MsgBuffer* buf) {
    return MsgBuffer_GetTextBoxDisplayIcon(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetTextBoxDisplayIcon(MsgBuffer* buf, u8 icon) {
    MsgBuffer_SetTextBoxDisplayIcon(buf, icon);
}
RECOMP_EXPORT u16 EZTR_MsgBuffer_GetNextMsg(MsgBuffer* buf) {
    return MsgBuffer_GetNextMsg(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetNextMsg(MsgBuffer* buf, u16 type) {
    MsgBuffer_SetNextMsg(buf, type);
}
RECOMP_EXPORT u16 EZTR_MsgBuffer_GetFirstItemRupees(MsgBuffer* buf) {
    return MsgBuffer_GetFirstItemRupees(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetFirstItemRupees(MsgBuffer* buf, u16 pos) {
    MsgBuffer_SetFirstItemRupees(buf, pos);
}
RECOMP_EXPORT u16 EZTR_MsgBuffer_GetSecondItemRupees(MsgBuffer* buf) {
    return MsgBuffer_GetSecondItemRupees(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_SetSecondItemRupees(MsgBuffer* buf, u16 pos) {
    MsgBuffer_SetSecondItemRupees(buf, pos);
}
RECOMP_EXPORT void EZTR_MsgBuffer_Print(MsgBuffer* buf) {
    MsgBuffer_Print(buf);
}
RECOMP_EXPORT void EZTR_MsgBuffer_PrintFull(MsgBuffer* buf) {
    MsgBuffer_Print(buf);
}
RECOMP_EXPORT char* EZTR_MsgBuffer_GetContentPtr(MsgBuffer* buf) {
    return MsgBuffer_GetContentPtr(buf);
}

RECOMP_EXPORT void EZTR_MsgSContent_SetEmpty(char* cont) {
    MsgSContent_SetEmpty(cont);
}
RECOMP_EXPORT u32 EZTR_MsgSContent_Len(char* cont) {
    return MsgSContent_Len(cont);
}

RECOMP_EXPORT u32 EZTR_MsgSContent_Copy(char* dst, char* src) {
    return MsgSContent_Copy(dst, src);
}
RECOMP_EXPORT u32 EZTR_MsgSContent_NCopy(char* dst, char* src, size_t len) {
    return MsgSContent_NCopy(dst, src, len);
}
RECOMP_EXPORT char* EZTR_MsgSContent_NCat(char* dst, char* src, size_t len) {
    return MsgSContent_NCat(dst, src, len);
}
RECOMP_EXPORT char* EZTR_MsgSContent_Cat(char* dst, char* src) {
    return MsgSContent_Cat(dst, src);
}
RECOMP_EXPORT s32 EZTR_MsgSContent_NCmp(char* str1, char* str2, size_t len) {
    return MsgSContent_NCmp(str1, str2, len);
}
RECOMP_EXPORT s32 EZTR_MsgSContent_Cmp(char* str1, char* str2) {
    return MsgSContent_Cmp(str1, str2);
}

// Can't really pass the extended args. Gotta reimplement here.
RECOMP_EXPORT int EZTR_MsgSContent_Printf(const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    char buffer[1];
    const int ret = _MsgSContent_Vsnprintf(_out_char, buffer, MSG_CONTENT_SIZE, (const char*)format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}

RECOMP_EXPORT int EZTR_MsgSContent_Sprintf(char* buffer, const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    const int ret = _MsgSContent_Vsnprintf(_out_buffer, buffer, MSG_CONTENT_SIZE, format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}


RECOMP_EXPORT int EZTR_MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    const int ret = _MsgSContent_Vsnprintf(_out_buffer, buffer, count, format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}


RECOMP_EXPORT int EZTR_MsgSContent_Vprintf(const char* format, va_list va) {
    char buffer[1];
    return _MsgSContent_Vsnprintf(_out_char, (char*)buffer, (size_t)-1, format, (size_t)-1, va);
}


RECOMP_EXPORT int EZTR_MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va) {
    return _MsgSContent_Vsnprintf(_out_buffer, buffer, count, format, (size_t)-1, va);
}


RECOMP_EXPORT int EZTR_MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...) {
    va_list va;
    pf_va_start(va, format);
    const out_fct_wrap_type out_fct_wrap = { out, arg };
    const int ret = _MsgSContent_Vsnprintf(_out_fct, (char*)(uintptr_t)&out_fct_wrap, (size_t)-1, format, (size_t)-1, va);
    pf_va_end(va);
    return ret;
}
