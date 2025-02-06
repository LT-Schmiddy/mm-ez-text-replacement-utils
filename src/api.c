#include "api.h"

MsgTable* table;

RECOMP_DECLARE_EVENT(EZTR_OnDeclareTextReplacement());

RECOMP_EXPORT void EZTR_ReplaceText(s16 textId,
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees,
    bool pipe_escape_bytes,
    char* content
) {
    MsgBuffer buf;
    memcpy(&buf.schar[0], &text_box_type, sizeof(u8));
    memcpy(&buf.schar[1], &text_box_y_pos, sizeof(u8));
    memcpy(&buf.schar[2], &display_icon, sizeof(u8));
    memcpy(&buf.schar[3], &next_message_id, sizeof(u16));
    memcpy(&buf.schar[5], &first_item_rupees, sizeof(u16));
    memcpy(&buf.schar[7], &second_item_rupees, sizeof(u16));

    if (pipe_escape_bytes) {
        MsgBuffer_CopyFromCharStr_PipeEscapeBytes(&buf, content);
    } else {
        MsgBuffer_CopyFromCharStr(&buf, content);
    }
    
    MsgTable_SetBuffer(table, textId, &buf);
}

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    table = MsgTable_Create();
    EZTR_OnDeclareTextReplacement();
}

RECOMP_HOOK_RETURN("Message_OpenText") void handle_text_replacement(PlayState* play, u16 p_textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    
    u16 textId = msgCtx->currentTextId;

    MsgEntry* entry = MsgTable_GetEntry(table, textId);
    IF_DEBUG recomp_printf("Message_OpenText Hook: %d\n", textId); 
    // Handled text replacement
    if (entry != NULL) {
        recomp_printf("Replacing text %d.\n", textId);        
        msgCtx->msgLength = entry->len + 1;
        IF_DEBUG recomp_printf("msgCtx->msgLength: %i\n", msgCtx->msgLength); 
        
        // Not completely sure, but using memcpy causes issues if the message is too long.
        // We'll just copy char by char.
        for (int i = 0; i < entry->len; i++) {
            font->msgBuf.schar[i] = entry->buf.schar[i];
        }

        // print_buf(&font->msgBuf, entry->len);
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
