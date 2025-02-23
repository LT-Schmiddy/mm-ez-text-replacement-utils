#include "hooks.h"

PlayState* Message_OpenText_stored_play = NULL;
RECOMP_HOOK("Message_OpenText") void before_handle_main_text_replacement(PlayState* play, u16 p_textId) {
    Message_OpenText_stored_play = play;
}

// RECOMP_HOOK_RETURN("Message_OpenText") void handle_main_text_replacement(PlayState* play, u16 p_textId) {
RECOMP_HOOK_RETURN("Message_OpenText") void handle_main_text_replacement() {
    
    MessageContext* msgCtx = &Message_OpenText_stored_play->msgCtx;
    Font* font = &msgCtx->font;
    u16 textId = msgCtx->currentTextId;
    LOGV_F("Message_OpenText Hook: 0x%04X (%i).", (u32)textId, (u32)textId);

    if (TEXT_DUMPING) {
        dump_buffer("Game", textId, msgCtx->msgLength, (MsgBuffer*)&font->msgBuf);
    }

    MsgBuffer* buf = MsgTable_LoadBufferCallback(ETZR_mainTable, textId, Message_OpenText_stored_play);

    // Handled text replacement
    if (buf != NULL) {
        LOGI_F("Replacing Text 0x%04X (%i).", (u32)textId, (u32)textId);     
        // MsgTable_RunCallback(ETZR_mainTable, msgCtx->currentTextId, play);

        msgCtx->msgLength = MsgBuffer_Len(buf) + 1;
        MsgBuffer_Copy((MsgBuffer*)&font->msgBuf, buf->raw.schar);
        MsgBuffer_Destroy(buf);

        LOGV_F("msgCtx->msgLength: %i\n", msgCtx->msgLength); 
        IF_LOG_VERBOSE MsgBuffer_Print((MsgBuffer*)&font->msgBuf);
        // Not completely sure, but using memcpy causes issues if the message is too long.
        // We'll just copy char by char.


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
            (Message_OpenText_stored_play->pauseCtx.bombersNotebookOpen)) {
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

// Handles the text for item descriptions:
PlayState* func_801514B0_stored_play;
u8 func_801514B0_stored_arg2;
RECOMP_HOOK("func_801514B0") void before_handle_item_desc(PlayState* play, u16 arg1, u8 arg2) {
    func_801514B0_stored_play = play;
    func_801514B0_stored_arg2 = arg2;
}

RECOMP_HOOK_RETURN("func_801514B0") void handle_item_desc() {
    PlayState* play = func_801514B0_stored_play;
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    
    if (TEXT_DUMPING) {
        dump_buffer("Game", msgCtx->currentTextId, msgCtx->msgLength, (MsgBuffer*)&font->msgBuf);
    }

    MsgBuffer* buf = MsgTable_LoadBufferCallback(ETZR_mainTable, msgCtx->currentTextId, play);
    if (buf != NULL) {
        LOGI_F("Replacing Text 0x%04X (%i).", (u32)msgCtx->currentTextId, (u32)msgCtx->currentTextId);     

        msgCtx->msgLength = MsgBuffer_Len(buf) + 1;
        MsgBuffer_Copy((MsgBuffer*)&font->msgBuf, buf->raw.schar);
        MsgBuffer_Destroy(buf);

        LOGV_F("msgCtx->msgLength: %i\n", msgCtx->msgLength); 
        IF_LOG_VERBOSE MsgBuffer_Print((MsgBuffer*)&font->msgBuf);

        msgCtx->choiceNum = 0;
        msgCtx->textUnskippable = false;
        msgCtx->textboxEndType = TEXTBOX_ENDTYPE_DEFAULT;
        msgCtx->textDrawPos = 0;
        msgCtx->msgBufPos = 0;
        msgCtx->decodedTextLen = 0;
        msgCtx->unk11F08 = font->msgBuf.wchar[msgCtx->msgBufPos];
        msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
        msgCtx->textBoxType = TEXTBOX_TYPE_9;
        msgCtx->textBoxPos = func_801514B0_stored_arg2;
        msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;
        msgCtx->textUnskippable = true;
        // DmaMgr_RequestSync(msgCtx->textboxSegment, SEGMENT_ROM_START(message_static) + (D_801CFC78[0] * 0x1000), 0x1000);
        msgCtx->textboxColorRed = 0;
        msgCtx->textboxColorGreen = 0;
        msgCtx->textboxColorBlue = 0;
        msgCtx->textboxColorAlphaTarget = 0xE6;
        msgCtx->textboxColorAlphaCurrent = 0;
        msgCtx->textColorAlpha = 0xFF;
        msgCtx->talkActor = NULL;
        msgCtx->msgMode = MSGMODE_TEXT_START;
        msgCtx->stateTimer = 0;
        msgCtx->textDelayTimer = 0;
        play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
        msgCtx->unk120D2 = 0xFF;
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2;
    }
}

