#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall NetCogibleDdl_CopyFlagAndCheckActive(void* self, void* /*edx*/, uint8_t* outFlag)
{
    if (outFlag) {
        *outFlag = CSF_AT(uint8_t, self, 0x6F2);
    }
    if (CSF_AT(uint8_t, self, 0x6F0) != 0 && CSF_AT(uint8_t, self, 0x443) != 0) {
        return 1;
    }
    return 0;
}

HOOK(0x00740DA0, NetCogibleDdl_CopyFlagAndCheckActive, "auto");
