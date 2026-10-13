#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_SetSetupPropertyD(C_00833908* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x190) = value;
    return value;
}

HOOK(0x49EBE0, C_00833908_SetSetupPropertyD, "auto");

