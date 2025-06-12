#include "msg_table_custom_handles.h"
#include "recomp_logging.h"

bool CustomMsgHandle_AssignAndValidate(CustomMsgHandle handle, u16 textId) {
    CustomMsgHandleSetter s;
    s.new_id = textId;
    u16 result = handle(&s);

    if (s.out_success) {
        if (result != textId) {
            LOGE("A CustomMsgHandle reports that assignment was successful, but did not return the assigned message ID.");
            return false;
        }
    }

    return s.out_success;

}