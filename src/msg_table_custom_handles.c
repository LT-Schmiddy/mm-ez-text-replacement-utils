#include "msg_table_custom_handles.h"
#include "recomp_logging.h"

u16 CustomMsgHandle_NextMessageId = MSG_HIGHEST_ID + 1;;

bool CustomMsgHandle_AssignAndValidate(CustomMsgHandle handle) {
    CustomMsgHandleSetter s;
    s.new_id = CustomMsgHandle_NextMessageId;
    u16 result = handle(&s);

    if (s.out_success) {
        if (result != CustomMsgHandle_NextMessageId) {
            LOGE("A CustomMsgHandle reports that assignment was successful, but did not return the assigned message ID.");
            return false;
        } else {
            CustomMsgHandle_NextMessageId++;
        }
    } else {
        LOGD("CustomMsgHandle Assignment Failure. This handle may have already been assigned. Skipping.");
    }
    return s.out_success;
}