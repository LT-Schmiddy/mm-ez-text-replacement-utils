#include "dumping.h"

void message_id_as_hex(u16 id, char* out_str) {
    u8 id_parts[2];
    memcpy(&id_parts, &id, sizeof(u16));
    
    write_byte_to_hex(id_parts[0], &out_str[0]);
    write_byte_to_hex(id_parts[1], &out_str[2]);
}

void dump_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf) {

    // Loading Header Info:
    u8 text_box_type = 0;
    u8 text_box_y_pos = 0;
    u8 display_icon = 0;
    u16 next_message_id = 0xffff;
    u16 first_item_rupees = 0xffff;
    u16 second_item_rupees = 0xffff;

    memcpy(&text_box_type, &buf->schar[0], sizeof(u8));
    memcpy(&text_box_y_pos, &buf->schar[1], sizeof(u8));
    memcpy(&display_icon, &buf->schar[2], sizeof(u8));
    memcpy(&next_message_id, &buf->schar[3], sizeof(u16));
    memcpy(&first_item_rupees, &buf->schar[5], sizeof(u16));
    memcpy(&second_item_rupees, &buf->schar[7], sizeof(u16));

    // Hex conversions:
    char id_str[5] ="0000";
    message_id_as_hex(textId, id_str);

    // Printing Output:
    recomp_printf("\nDumping %s Message ID 0x%s (%i):\n", category, id_str, textId);
    recomp_printf("\tTextbox Type: %s\n", eztr_textbox_names[text_box_type]);
    recomp_printf("\tTextbox Y Position: %i\n", text_box_y_pos);
    recomp_printf("\tDisplay Icon: %s\n", eztr_icon_names[display_icon]);

    if (next_message_id == 0xffff) {
        recomp_printf("\tNext Message ID: EZTR_NO_VALUE\n");
    } else {
        char next_id_str[5] = "0000";
        message_id_as_hex(next_message_id, next_id_str);
        recomp_printf("\tNext Message ID: 0x%s\n", next_id_str);
    }
    if (first_item_rupees == 0xffff) {
        recomp_printf("\tFirst Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tFirst Item Rupees: %i\n", first_item_rupees);
    }

    if (second_item_rupees == 0xffff) {
        recomp_printf("\tSecond Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tSecond Item Rupees: %i\n", second_item_rupees);
    }

    recomp_printf("\tContent: \"");
    // Printing Message Content:
    for (s32 i = MESSAGE_HEADER_SIZE; i < len; i++) {
        char c = buf->schar[i];

        if (is_printable_char(c)) {
            recomp_printf("%c", c);
        } else {
            // converting to hex
            char out_str[4] = "|00";
            write_byte_to_hex(c,&out_str[1]);
            recomp_printf("%s", out_str);
        }
    }

     recomp_printf("\"\n\n");
}

void dump_full_buffer(const char* category, u16 textId, s32 len, MsgBuffer* buf) {

    // Loading Header Info:
    u8 text_box_type = 0;
    u8 text_box_y_pos = 0;
    u8 display_icon = 0;
    u16 next_message_id = 0xffff;
    u16 first_item_rupees = 0xffff;
    u16 second_item_rupees = 0xffff;

    memcpy(&text_box_type, &buf->schar[0], sizeof(u8));
    memcpy(&text_box_y_pos, &buf->schar[1], sizeof(u8));
    memcpy(&display_icon, &buf->schar[2], sizeof(u8));
    memcpy(&next_message_id, &buf->schar[3], sizeof(u16));
    memcpy(&first_item_rupees, &buf->schar[5], sizeof(u16));
    memcpy(&second_item_rupees, &buf->schar[7], sizeof(u16));

    // Hex conversions:
    char id_str[5] ="0000";
    message_id_as_hex(textId, id_str);

    // Printing Output:
    recomp_printf("\nDumping %s Message ID 0x%s (%i):\n", category, id_str, textId);
    recomp_printf("\tTextbox Type: %s\n", eztr_textbox_names[text_box_type]);
    recomp_printf("\tTextbox Y Position: %i\n", text_box_y_pos);
    recomp_printf("\tDisplay Icon: %s\n", eztr_icon_names[display_icon]);

    if (next_message_id == 0xffff) {
        recomp_printf("\tNext Message ID: EZTR_NO_VALUE\n");
    } else {
        char next_id_str[5] = "0000";
        message_id_as_hex(next_message_id, next_id_str);
        recomp_printf("\tNext Message ID: 0x%s\n", next_id_str);
    }
    if (first_item_rupees == 0xffff) {
        recomp_printf("\tFirst Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tFirst Item Rupees: %i\n", first_item_rupees);
    }

    if (second_item_rupees == 0xffff) {
        recomp_printf("\tSecond Item Rupees: EZTR_NO_VALUE\n");
    } else {
        recomp_printf("\tSecond Item Rupees: %i\n", second_item_rupees);
    }

    recomp_printf("\tContent: \"");
    // Printing Message Content:
    for (s32 i = 0; i < MESSAGE_BUFFER_SIZE; i++) {
        char c = buf->schar[i];

        if (is_printable_char(c)) {
            recomp_printf("%c", c);
        } else {
            // converting to hex
            char out_str[4] = "|00";
            write_byte_to_hex(c,&out_str[1]);
            recomp_printf("%s", out_str);
        }
    }

     recomp_printf("\"\n\n");
}