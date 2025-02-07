#include "api.h"

MsgTable* main_table;

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
    
    MsgTable_SetBuffer(main_table, textId, &buf);
}

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    main_table = MsgTable_Create();
    EZTR_OnDeclareTextReplacement();
}

RECOMP_HOOK_RETURN("Message_OpenText") void handle_main_text_replacement(PlayState* play, u16 p_textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    u16 textId = msgCtx->currentTextId;

    if (TEXT_DUMPING) {
        dump_buffer("Game", textId, msgCtx->msgLength, (MsgBuffer*)&font->msgBuf);
    }

    MsgBuffer* buf = MsgTable_GetBuffer(main_table, textId);
    IF_DEBUG recomp_printf("Message_OpenText Hook: %d\n", textId); 
    // Handled text replacement
    if (buf != NULL) {
        recomp_printf("Replacing Text %d.\n", textId);        
        msgCtx->msgLength = MsgBuffer_Len(buf);
        IF_DEBUG recomp_printf("msgCtx->msgLength: %i\n", msgCtx->msgLength); 
        
        // Not completely sure, but using memcpy causes issues if the message is too long.
        // We'll just copy char by char.
        for (int i = 0; i < msgCtx->msgLength; i++) {
            font->msgBuf.schar[i] = buf->schar[i];
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

    MsgBuffer* buf = MsgTable_GetBuffer(main_table, msgCtx->currentTextId);
    if (buf != NULL) {
        recomp_printf("Replacing Text %d.\n", msgCtx->currentTextId);        
        msgCtx->msgLength = MsgBuffer_Len(buf);
        for (int i = 0; i < msgCtx->msgLength; i++) {
            font->msgBuf.schar[i] = buf->schar[i];
        }
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