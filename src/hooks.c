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

    if (TEXT_DUMPING) {
        dump_buffer("Game", textId, msgCtx->msgLength, (MsgBuffer*)&font->msgBuf);
    }

    MsgBuffer* buf = MsgTable_LoadBufferCallback(ETZR_mainTable, textId, Message_OpenText_stored_play);
    LOGV_F("Message_OpenText Hook: 0x%04X (%i).", (u32)textId, (u32)textId);
    // Handled text replacement
    if (buf != NULL) {
        LOGI_F("Replacing Text 0x%04X (%i).", (u32)textId, (u32)textId);     
        // MsgTable_RunCallback(ETZR_mainTable, msgCtx->currentTextId, play);

        msgCtx->msgLength = MsgBuffer_Len(buf) + 1;
        // for (int i = 0; i < msgCtx->msgLength+1; i++) {
        //     font->msgBuf.schar[i] = buf->raw.schar[i];
        // }
        MsgBuffer_Copy((MsgBuffer*)&font->msgBuf, buf->raw.schar);
        // MsgBuffer_Print((MsgBuffer*)&font->msgBuf);
        MsgBuffer_Destroy(buf);
        LOGV_F("msgCtx->msgLength: %i\n", msgCtx->msgLength); 
        
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

extern s32 sCharTexSize;
extern s32 sCharTexScale;
extern s32 D_801F6B08;
extern u8 D_801C6A70;
extern s16 D_801CFC78[16];
void Message_FindMessage(PlayState* play, u16 textId);

RECOMP_PATCH void func_801514B0(PlayState* play, u16 arg1, u8 arg2) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 temp = 1024.0f;

    msgCtx->ocarinaAction = 0xFFFF;

    if (msgCtx->msgMode == MSGMODE_NONE) {
        gSaveContext.prevHudVisibility = gSaveContext.hudVisibility;
    }
    msgCtx->messageHasSetSfx = false;
    D_801C6A70 = 0;
    msgCtx->textboxSkipped = false;
    msgCtx->textIsCredits = false;

    if (gSaveContext.options.language == LANGUAGE_JPN) {
        msgCtx->textCharScale = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->textCharScale = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }

    sCharTexSize = msgCtx->textCharScale * 16.0f;
    sCharTexScale = temp / msgCtx->textCharScale;
    D_801F6B08 = temp / 1;

    if ((arg1 == 0x1709) && (player->transformation == 3)) {
        arg1 = 0x1705;
    }

    msgCtx->currentTextId = arg1;

    if (gSaveContext.options.language == LANGUAGE_JPN) {
        Message_FindMessage(play, arg1);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_RequestSync(&font->msgBuf, SEGMENT_ROM_START(message_data_static) + font->messageStart,
                           font->messageEnd);
    } else {
        Message_FindMessageNES(play, arg1);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_RequestSync(&font->msgBuf, SEGMENT_ROM_START(message_data_static) + font->messageStart,
                           font->messageEnd);
    }

    if (TEXT_DUMPING) {
        dump_buffer("Game", msgCtx->currentTextId, msgCtx->msgLength, (MsgBuffer*)&font->msgBuf);
    }

    MsgBuffer* buf = MsgTable_LoadBufferCallback(ETZR_mainTable, msgCtx->currentTextId, play);
    if (buf != NULL) {
        LOGI_F("Replacing Text %d.\n", msgCtx->currentTextId);        
        // running Callbacks:
        // MsgTable_RunCallback(ETZR_mainTable, msgCtx->currentTextId, play);

        // msgCtx->msgLength = MsgBuffer_StrCopy(font->msgBuf.schar, (char*)buf);
        // msgCtx->msgLength = MsgBuffer_StrCopy(font->msgBuf.schar, (char*)buf);
        msgCtx->msgLength = MsgBuffer_Len(buf);
        for (int i = 0; i < msgCtx->msgLength + 1; i++) {
            font->msgBuf.schar[i] = buf->raw.schar[i];
        }
        MsgBuffer_Destroy(buf);
    }

    msgCtx->choiceNum = 0;
    msgCtx->textUnskippable = false;
    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_DEFAULT;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;
    msgCtx->unk11F08 = font->msgBuf.wchar[msgCtx->msgBufPos];
    msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
    msgCtx->textBoxType = TEXTBOX_TYPE_9;
    msgCtx->textBoxPos = arg2;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;
    msgCtx->textUnskippable = true;
    DmaMgr_RequestSync(msgCtx->textboxSegment, SEGMENT_ROM_START(message_static) + (D_801CFC78[0] * 0x1000), 0x1000);
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