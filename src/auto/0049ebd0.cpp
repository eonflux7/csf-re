#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_SetSetupPropertyC(C_00833908* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x18C) = value;
    return value;
}

HOOK(0x49EBD0, C_00833908_SetSetupPropertyC, "auto");
