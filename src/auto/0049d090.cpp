#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall SetFieldAt1B4(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x1B4) = value;
    return value;
}

HOOK(0x49D090, SetFieldAt1B4, "auto");
