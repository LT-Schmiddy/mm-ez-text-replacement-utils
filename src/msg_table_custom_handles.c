#include "msg_table_custom_handles.h"

u16 CustomMsgHandle_GetId(CustomMsgHandle handle) {
    return handle(NULL);
}

void CustomMsgHandle_SetId(CustomMsgHandle handle, u16 new_id) {
    handle(&new_id);
}