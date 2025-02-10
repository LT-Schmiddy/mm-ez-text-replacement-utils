#include "basic_api.h"

MsgTable* ETZR_mainTable;

RECOMP_DECLARE_EVENT(EZTR_OnInit());

RECOMP_EXPORT void EZTR_Basic_ReplaceText(u16 textId,
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
        MsgBuffer_CreateFromStr_PipeEscapeBytes(&buf, content);
    } else {
        MsgBuffer_CreateFromStr(&buf, content);
    }
    recomp_printf("Adding buffer %X\n", (u32)textId);
    MsgTable_SetBuffer(ETZR_mainTable, textId, &buf);
}

void test_callback(PlayState* play, u16 textId, MsgBuffer* buf) {
    recomp_printf("Test Callback on text id: %X\n", (u32)textId);
}

RECOMP_CALLBACK("*", recomp_on_init) void setup_table () {
    ETZR_mainTable = MsgTable_Create();
    EZTR_OnInit();

    printf("Hello Alex %i, %02f, %c, %s", 1, 2.0f, 'c', "How are you?\n");

    EZTR_Basic_ReplaceText(
        0x0C01,
        EZTR_STANDARD_TEXT_BOX_I,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "|17You can |01save|00 your |01progress|00 and|11|01quit|00 here. When you|11reselect your file, my face will|11appear by your file name.|18|10|17This indicates that the next time|11you reopen your file, you'll resume|11playing at this |01very place|00 and|11|01time|00 with your |01current status|00.|18|10|01Save|00 your |01progress|00 up to this|11point and |01quit|00 playing?|11|02|C2No|11Yes|BF|00"
    );

    MsgTable_SetCallback(ETZR_mainTable, 0x0C01, test_callback);
}
