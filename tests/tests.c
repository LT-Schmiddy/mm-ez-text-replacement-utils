#include <modding.h>
#include <eztr_api.h>
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
    recomp_printf("This buffer should have default values and no content.\n");
    EZTR_MsgBuffer_Print(buf1);

    EZTR_MsgBuffer_SetTextBoxType(buf1, EZTR_BLUE_TEXT_BOX);
    EZTR_MsgBuffer_SetTextBoxYPos(buf1, 1);
    EZTR_MsgBuffer_SetTextBoxDisplayIcon(buf1, EZTR_ICON_ADULT_WALLET);
    EZTR_MsgBuffer_SetNextMsg(buf1, 0x000F);
    EZTR_MsgBuffer_SetFirstItemRupees(buf1, 0x000F);
    EZTR_MsgBuffer_SetSecondItemRupees(buf1, 0x000F);
    
    recomp_printf("This buffer should have values set above and no content. Values to look for:\n");
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

    EZTR_MsgBuffer_Destroy(buf1);
    recomp_printf("Passed %i out of %i cases.\n", cases_passed, cases);
}