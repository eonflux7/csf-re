#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_SetSetupPropertyA(C_00833908* self, void* /*edx*/, int value) {
    CSF_AT(int, self, 0x184) = value;
    CSF_AT(int, self, 0x194) = value;
    return value;
}

HOOK(0x0049EBA0, C_00833908_SetSetupPropertyA, "auto");
