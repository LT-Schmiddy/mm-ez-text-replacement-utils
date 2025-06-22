#include <modding.h>
#include <eztr_api.h>


EZTR_ON_INIT void init_text() {
    // We'll test this by replacing the Bank Advertisement Sign:
    EZTR_Basic_ReplaceText(
        0x1C18,
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "Sign Version 3" EZTR_CC_END,
        NULL
    );
}