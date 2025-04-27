#include <modding.h>
#include <eztr_api.h>
#include "global.h"


#include "overlays/actors/ovl_En_Talk/z_en_talk.h"
RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

EZTR_DECLARE_CUSTOM_MSG_HANDLE(test_message);
EZTR_DECLARE_CUSTOM_MSG_HANDLE(test_message2);

// The whole purpose of this file is make the game try to load a textId that's
// outside of the vanilla range. Used to test custom messages:

// Actor* store_actor;
// PlayState* store_play;
// RECOMP_HOOK("EnTalk_Init") void EnTalk_Init_Before(Actor* thisx, PlayState* play) {
//     store_actor = thisx;
//     store_play = play;


// }

// RECOMP_HOOK("EnTalk_Init") void EnTalk_Init_After() {

//     EnTalk* this = (EnTalk*)store_actor;
//     // recomp_printf("REPLACEMENT_ID = 0x%02X\n", EZTR_test_message(NULL));
//     // this->actor.textId = EZTR_test_message(NULL);
// };

// RECOMP_PATCH void EnTalk_Update(Actor* thisx, PlayState* play) {
//     EnTalk* this = (EnTalk*)thisx;
//     // recomp_printf("REPLACEMENT_ID = 0x%02X\n", EZTR_test_message(NULL));
//     this->actor.textId = EZTR_HNAME(test_message2(NULL));
//     this->actionFunc(this, play);
// }
