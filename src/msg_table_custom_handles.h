#ifndef __MSG_TABLE_CUSTOM_HANDLES__ 
#define __MSG_TABLE_CUSTOM_HANDLES__

#include "mod_globals.h"

#define MSG_HIGHEST_ID 0x354C

// #define EZTR_CUSTOM_MSG_HANDLE_NAME(name_suffix) EZTR_CustomMsgHandle_##name_suffix
#define EZTR_CUSTOM_MSG_HANDLE_NAME(name) name

#define EZTR_DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT(name) \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id) \
{ static u16 id; if (new_id != NULL) { id = *new_id; } return id; }

#define EZTR_DEFINE_CUSTOM_MSG_HANDLE(name) RECOMP_EXPORT \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id) \
{ static u16 id; if (new_id != NULL) { id = *new_id; } return id; }

#define EZTR_EXTERN_CUSTOM_MSG_HANDLE(name) u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id);

#define EZTR_IMPORT_CUSTOM_MSG_HANDLE(mod_str, name) RECOMP_IMPORT(mod_str, u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id));

#define EZTR_HNAME(name) EZTR_CUSTOM_MSG_HANDLE_NAME(name)

// Type of the custom message handle:
typedef u16 (*CustomMsgHandle)(u16* new_id);

u16 CustomMsgHandle_GetId(CustomMsgHandle handle);

void CustomMsgHandle_SetId(CustomMsgHandle handle, u16 new_id);

#endif