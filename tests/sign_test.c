#include <modding.h>
#include <eztr_api.h>
#include "global.h"


#include "overlays/actors/ovl_En_Talk/z_en_talk.h"
RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

EZTR_DECLARE_CUSTOM_MSG_HANDLE(EZTZ_test_message);

Actor* store_actor;
PlayState* store_play;
RECOMP_HOOK("EnTalk_Init") void EnTalk_Init_Before(Actor* thisx, PlayState* play) {
    store_actor = thisx;
    store_play = play;


}

RECOMP_HOOK("EnTalk_Init") void EnTalk_Init_After() {

    EnTalk* this = (EnTalk*)store_actor;
    // recomp_printf("REPLACEMENT_ID = 0x%02X\n", EZTZ_test_message(NULL));
    // this->actor.textId = EZTZ_test_message(NULL);
};

RECOMP_PATCH void EnTalk_Update(Actor* thisx, PlayState* play) {
    EnTalk* this = (EnTalk*)thisx;
    // recomp_printf("REPLACEMENT_ID = 0x%02X\n", EZTZ_test_message(NULL));
    this->actor.textId = EZTZ_test_message(NULL);
    this->actionFunc(this, play);
}
