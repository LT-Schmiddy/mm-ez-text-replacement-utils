// #include <modding.h>
// #include <eztr_api.h>
// // #include <x_eztr_msg_macros.h>

// #include "global.h"
// #include "z64font.h"
// #include "z64message.h"

// RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));
// RECOMP_IMPORT("*", void* recomp_alloc(size_t size));
// RECOMP_IMPORT("*", void recomp_free(void* ptr));

// #define MESSAGE_SEARCH EZTR_MSG_HIGHEST_ID + 1
// #define CHAR_USAGE_TABLE_SIZE 256

// u32 char_usage_table[CHAR_USAGE_TABLE_SIZE];
// u32 msg_length_table[MESSAGE_SEARCH];


// RECOMP_HOOK("Message_OpenText") void scan_messages(PlayState* play, u16 textId) {
//     MessageContext* msgCtx = &play->msgCtx;
//     Font* font = &msgCtx->font;
    
//     // Initializing Values:
//     u32 total_msg_size = 0;
//     for (int i = 0; i < CHAR_USAGE_TABLE_SIZE; i++) {
//         char_usage_table[i] = 0;
//     }

//     for (u16 i = 0; i < MESSAGE_SEARCH; i++) {
//         msg_length_table[i] = 0;
//     }

//     recomp_printf("Analyzing Vanilla Messages 0x0000 through 0x%X...\n", EZTR_MSG_HIGHEST_ID);
//     for (u16 i = 0; i < MESSAGE_SEARCH; i++) {
//         // recomp_printf("ID = %02X", i);
//         Message_FindMessageNES(play, i);
//         // recomp_printf(", Message Found");
//         msgCtx->msgLength = font->messageEnd;
//         DmaMgr_RequestSync(&font->msgBuf, SEGMENT_ROM_START(message_data_static) + font->messageStart, font->messageEnd);
//         // recomp_printf(", Message Loaded\n");

//         EZTR_MsgBuffer* buf = (EZTR_MsgBuffer*) &font->msgBuf;
//         // +1 to count the termination character.
//         u32 message_length = EZTR_MsgBuffer_Len(buf) + 1;
//         // u32 message_length = EZTR_MsgBuffer_Len(buf);
//         total_msg_size += message_length;

//         // Assessing Character Usage:
//         for (u32 j = 0; j < message_length; j++) {
//             u8 chr = buf->raw.schar[j];
//             char_usage_table[chr] += 1;
//         }

//         msg_length_table[i] = message_length - 1 - EZTR_MSG_HEADER_SIZE;
//     }

//     recomp_printf("\nTotal Message Data: %i Bytes\n\n", total_msg_size);

//     // Empty Messages:
//     for (u16 i = 0; i < MESSAGE_SEARCH; i++) {
//         recomp_printf("0x%04X: %i\n", i, msg_length_table[i]);
//     }

//     recomp_printf("Empty Messages:\n");
//     for (u16 i = 0; i < MESSAGE_SEARCH; i++) {
//         if (msg_length_table[i] == 0) {
//             recomp_printf("0x%04X,\n", i);
//         }
//     }

//     // recomp_printf("Non-Empty Messages:\n");
//     // for (u16 i = 0; i < MESSAGE_SEARCH; i++) {
//     //     if (msg_length_table[i] != 0) {
//     //         recomp_printf("0x%04X\n", i);
//     //     }
//     // }

//     // Character Usage Rates:
//     // recomp_printf("\nCharacter Usage:\n");
//     // for (int i = 0; i < CHAR_USAGE_TABLE_SIZE; i++) {
//     //     EZTR_MsgSContent_PrintfLine("Character %c used %i times.\xBF", i, char_usage_table[i]);
//     // }
//     // recomp_printf("\nUnused Characters:\n");
//     // for (int i = 0; i < CHAR_USAGE_TABLE_SIZE; i++) {
//     //     if (char_usage_table[i] == 0) {
//     //         EZTR_MsgSContent_PrintfLine("%c\xBF", i);
//     //     }
//     // }
// }
