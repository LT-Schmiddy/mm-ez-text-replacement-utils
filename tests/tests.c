#include <modding.h>
#include <eztr_api.h>
#include <eztr_msg_macros.h>
#include "global.h"


RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));
RECOMP_IMPORT("*", void* recomp_alloc(size_t size));
RECOMP_IMPORT("*", void recomp_free(void* ptr));

RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char* key));
RECOMP_IMPORT("*", double recomp_get_config_double(const char* key));
RECOMP_IMPORT("*", char* recomp_get_config_string(const char* key));
RECOMP_IMPORT("*", void recomp_free_config_string(char* str));


int cases = 0;
int cases_passed = 0; 

EZTR_DEFINE_CUSTOM_MSG_HANDLE(test_message);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(test_message2);

EZTR_MSG_CALLBACK(my_callback) {
    static int i = 0;
    i++;
    EZTR_MsgSContent_Sprintf(buf->data.content, "You've read this %i times.\xBF", i);

    // Tests declaring a followup message:
    buf->data.next_message_id = EZTR_HNAME(test_message)(NULL);
}

void validate(char* case_name, bool case_stmt) {
    cases++;

    if (case_stmt) {
        cases_passed++;
    }

    recomp_printf("Case %s %s\n", case_name, case_stmt ? "Passed" : "Failed!");
}

EZTR_ON_INIT void run_tests() {
    recomp_printf("Running EZTR Tests:\n");

    EZTR_MsgBuffer* buf1 = EZTR_MsgBuffer_Create();
    recomp_printf("USER_CHECK: This buffer should have default values and no content.\n");
    EZTR_MsgBuffer_Print(buf1);


    // Testing Header Stuff:
    EZTR_MsgBuffer_SetTextBoxType(buf1, EZTR_BLUE_TEXT_BOX);
    EZTR_MsgBuffer_SetTextBoxYPos(buf1, 1);
    EZTR_MsgBuffer_SetTextBoxDisplayIcon(buf1, EZTR_ICON_ADULT_WALLET);
    EZTR_MsgBuffer_SetNextMsg(buf1, 0x000F);
    EZTR_MsgBuffer_SetFirstItemRupees(buf1, 0x000F);
    EZTR_MsgBuffer_SetSecondItemRupees(buf1, 0x000F);
    
    recomp_printf("USER_CHECK: This buffer should have values set above and no content. Values to look for:\n");
    recomp_printf("\tEZTR_BLUE_TEXT_BOX\n");
    recomp_printf("\t1\n");
    recomp_printf("\tEZTR_ICON_ADULT_WALLET\n");
    recomp_printf("\t0x000F\n");
    recomp_printf("\t0x000F\n");
    recomp_printf("\t0x000F\n");
    EZTR_MsgBuffer_Print(buf1);

    recomp_printf("\nTesting header getters:\n");
    validate("EZTR_MsgBuffer_GetTextBoxType", EZTR_MsgBuffer_GetTextBoxType(buf1) == EZTR_BLUE_TEXT_BOX);
    validate("EZTR_MsgBuffer_SetTextBoxYPos", EZTR_MsgBuffer_GetTextBoxYPos(buf1) == 1);
    validate("EZTR_MsgBuffer_GetTextBoxDisplayIcon", EZTR_MsgBuffer_GetTextBoxDisplayIcon(buf1) == EZTR_ICON_ADULT_WALLET);
    validate("EZTR_MsgBuffer_GetNextMsg", EZTR_MsgBuffer_GetNextMsg(buf1) == 0x000F);
    validate("EZTR_MsgBuffer_GetFirstItemRupees", EZTR_MsgBuffer_GetFirstItemRupees(buf1) == 0x000F);
    validate("EZTR_MsgBuffer_GetSecondItemRupees", EZTR_MsgBuffer_GetSecondItemRupees(buf1) == 0x000F);

    recomp_printf("\nTesting Header data struct packing:\n");
    validate("buf1->data.text_box_type", buf1->data.text_box_type == EZTR_BLUE_TEXT_BOX);
    validate("buf1->data.text_box_y_pos", buf1->data.text_box_y_pos == 1);
    validate("buf1->data.display_icon", buf1->data.display_icon == EZTR_ICON_ADULT_WALLET);
    validate("buf1->data.next_message_id", buf1->data.next_message_id == 0x000F);
    validate("buf1->data.first_item_rupees", buf1->data.first_item_rupees == 0x000F);
    validate("buf1->data.second_item_rupees", buf1->data.second_item_rupees == 0x000F);

    recomp_printf("\nTesting MsgSContent retrieval operations:\n");
    validate("uf1->data.content == buf1->partitions.content", buf1->data.content == buf1->partitions.content);
    validate("uf1->data.content == EZTR_MsgBuffer_GetContentPtr", buf1->data.content == EZTR_MsgBuffer_GetContentPtr(buf1));


    // Testing Buffer Stuff:
    recomp_printf("\nTesting MsgBuffer operations:\n");
    EZTR_MsgBuffer* buf2 = EZTR_MsgBuffer_Create();
    EZTR_MsgBuffer* buf3 = EZTR_MsgBuffer_Create();

    validate("EZTR_MsgSContent_Cmp: Equality (Empty Content)", EZTR_MsgSContent_Cmp(buf2->data.content, buf3->data.content) == 0);
    buf2->data.text_box_y_pos = 1;
    validate("EZTR_MsgSContent_NCmp: Inequality (buf2 has a higher textbox pos)", 1 == EZTR_MsgSContent_NCmp(buf2->raw.schar, buf3->raw.schar, EZTR_MSG_HEADER_SIZE));
    
    buf2->data.text_box_y_pos = 0;
    u32 b2count = EZTR_MsgSContent_Copy(buf2->data.content, "Hello Alex\xBF");
    // recomp_printf("b2count = %u\n", b2count);
    validate("Count == 10", b2count == 10);

    EZTR_MsgSContent_Copy(buf3->data.content, "Hello Alex\xBF");
    EZTR_MsgBuffer_PrintFull(buf3);

    recomp_printf("USER_CHECK: These two buffers should have the same values.");
    EZTR_MsgBuffer_Print(buf2);
    EZTR_MsgBuffer_Print(buf3);

    validate("EZTR_MsgSContent_Cmp: Equality (Content)", 0 == EZTR_MsgSContent_Cmp(buf2->data.content, buf3->data.content));
    EZTR_MsgSContent_Copy(buf3->data.content, "Hello Alex Schmid\xBF");
    validate("EZTR_MsgSContent_Cmp: Mismatch (buf3 is longer)", -1 == EZTR_MsgSContent_Cmp(buf2->data.content, buf3->data.content));
    buf3->data.content[0] = '\0';
    validate("EZTR_MsgSContent_Cmp: Mismatch (buf3 is longer, but with an earlier mismatch)", 1 == EZTR_MsgSContent_Cmp(buf2->data.content, buf3->data.content));
    
    buf3->data.content[0] = 'H';
    validate("EZTR_MsgSContent_Cat", 0 == EZTR_MsgSContent_Cmp(EZTR_MsgSContent_Cat(buf2->data.content, " Schmid\xBF"), buf3->data.content));

    // At this point, we'll consider that EZTR_MsgSContent_Cmp works as intended.
    EZTR_MsgSContent_Sprintf(buf2->data.content, "Hello Alex %m\xBF", "Schmid\xBF");
    validate("EZTR_MsgSContent_Sprintf", 0 == EZTR_MsgSContent_Cmp(buf2->data.content, buf3->data.content));

    // Custom Message Stuff:
    EZTR_Basic_AddCustomText(EZTR_HNAME(test_message), EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO ALEX\xBF", NULL);

    validate("my_message id == 0x354D", EZTR_HNAME(test_message)(NULL) == 0x354D);
    
    // This should fail.
    EZTR_Basic_AddCustomText(EZTR_HNAME(test_message), EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO ALEX 2\xBF", NULL);
    
    // So should this:
    EZTR_Basic_ReplaceText(EZTR_GET_ID(EZTR_HNAME(test_message)) , EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO ALEX 2\xBF", NULL);
    
    // This should work.
    EZTR_Basic_ReplaceCustomText(EZTR_HNAME(test_message), EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
            EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO ALEX 3\xBF", NULL);
            
    EZTR_Basic_AddCustomTextEmpty(EZTR_HNAME(test_message2), my_callback);
    
    validate("EZTR_test_message2 id == 0x354D", EZTR_GET_ID(EZTR_HNAME(test_message2)) == 0x354E);
    recomp_printf("EZTR_test_message2 id = 0x%04X\n", EZTR_GET_ID(EZTR_HNAME(test_message2)));

    // Deku Nut message. Tests item description replacement.
    EZTR_Basic_ReplaceText(0x1709, EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, EZTR_MSG_COLOR_RED "HELLO ALEX 4"EZTR_MSG_COLOR_DEFAULT EZTR_MSG_END, NULL);


    EZTR_MsgBuffer_Destroy(buf1);
    EZTR_MsgBuffer_Destroy(buf2);
    EZTR_MsgBuffer_Destroy(buf3);
    recomp_printf("Passed %i out of %i cases.\n", cases_passed, cases);
}
