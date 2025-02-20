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
    
    EZTR_MsgBuffer_WriteDefaultHeader(buf1);
    

    EZTR_MsgBuffer_Destroy(buf1);
    recomp_printf("Passed %i out of %i cases.\n", cases_passed, cases);
}