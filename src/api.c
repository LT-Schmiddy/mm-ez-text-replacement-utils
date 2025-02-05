#include "mod_globals.h"
#include "msg_table.h"
#include "msg_table_quicksort.h"
#include "libc/string.h"
#include "util.h"


MsgTable* table;

RECOMP_DECLARE_EVENT(OnDeclareTextReplacement());



RECOMP_EXPORT void EZTR_ReplaceText(s16 textId,
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees,
    char* content
) {
    MsgBuffer buf;
    memcpy(&buf.schar[0], &text_box_type, sizeof(u8));
    memcpy(&buf.schar[1], &text_box_y_pos, sizeof(u8));
    memcpy(&buf.schar[2], &display_icon, sizeof(u8));
    memcpy(&buf.schar[3], &next_message_id, sizeof(u16));
    memcpy(&buf.schar[5], &first_item_rupees, sizeof(u16));
    memcpy(&buf.schar[7], &second_item_rupees, sizeof(u16));

    // MsgBuffer_CopyFromCharStr(&buf, content);
    MsgBuffer_CopyFromCharStr_PipeEscapeBytes(&buf, content);
    recomp_printf("EZ Add %i\n", textId);
    MsgTable_SetBuffer(table, textId, &buf);

}

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    table = MsgTable_Create();
    OnDeclareTextReplacement();
    char byte = '\xcb';
    char hex_out[3];

    write_byte_to_hex(byte, hex_out);
    hex_out[2] = '\0';

    u8 out = 0; 
    out = hex_to_byte(hex_out);

    recomp_printf("Testing hex parsing -> \\x%s\n", hex_out);
    recomp_printf("Testing hex parsing -> \\x%i\n", out);

    EZTR_ReplaceText(0x0C01, 0, 1, 254, 0xffff, 0xffff, 0xffff, "|17|10|01Save|00 your |01progress|00 up to this|11point and |01quit|00 playing?|11|02|C2No|11Yes|BF|00");
    // EZTR_ReplaceText(0x0C01, 0, 1, 254, 0xffff, 0xffff, 0xffff, "\x17You can \x01save\x00 your \x01progress\x00 and\x11\x01quit\x00 here. When you\x11reselect your file, my face will\x11 appear by your file name.\x18\x10\x17This indicates that the next time\x11you reopen your file, you'll resume\x11playing at this \x01very place\x00 and\x11\x01time\x00 with your \x01current status\x00.\x18\x10\x01Save\x00 your \x01progress\x00 up to this\x11point and \x01quit\x00 playing?\x11\x02\xC2No\x11Yes\xBF\x00");
    // EZTR_ReplaceText(0x0C01, 0, 1, 254, 0xffff, 0xffff, 0xffff, "\x17Quit Playing?\x11\x02\xC2No\x11Yes\xBF\x00");
}


RECOMP_HOOK_RETURN("Message_OpenText") void handle_text_replacement(PlayState* play, u16 p_textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    
    u16 textId = msgCtx->currentTextId;

    MsgEntry* entry = MsgTable_GetEntry(table, textId);
    recomp_printf("Message_OpenText Hook: %d\n", textId); 
    // Handled text replacement
    if (entry != NULL) {
        recomp_printf("Replacing text %d.\n", textId);        
        msgCtx->msgLength = MsgTable_GetBufferLen(table, textId);

        memcpy(font->msgBuf.schar, entry->buf.schar, sizeof(MsgBuffer));

        // Copied from Message_OpenText, processing for message header.
        msgCtx->choiceNum = 0;
        msgCtx->textUnskippable = false;
        msgCtx->textboxEndType = TEXTBOX_TYPE_0;
        msgCtx->textDrawPos = 0;
        msgCtx->msgBufPos = 0;
        msgCtx->decodedTextLen = 0;

        msgCtx->unk11F08 = font->msgBuf.schar[msgCtx->msgBufPos] << 8;
        msgCtx->unk11F08 |= font->msgBuf.schar[msgCtx->msgBufPos + 1];

        msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
        msgCtx->textBoxType = (msgCtx->unk11F08 & 0xF00) >> 8;
        msgCtx->textBoxPos = (msgCtx->unk11F08 & 0xF0) >> 4;
        msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;

        if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
            msgCtx->textUnskippable = true;
        }
        msgCtx->itemId = 0xFE;

        if ((msgCtx->textBoxType == TEXTBOX_TYPE_5) || (msgCtx->textBoxType == TEXTBOX_TYPE_D) ||
            (play->pauseCtx.bombersNotebookOpen)) {
            msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0;
        } else {
            msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0xFF;
        }

        msgCtx->choiceIndex = 0;

        if (msgCtx->unk11F0C != 3) {
            msgCtx->textColorAlpha = 0xFF;
        } else {
            msgCtx->textColorAlpha = 0;
        }
    }
}
