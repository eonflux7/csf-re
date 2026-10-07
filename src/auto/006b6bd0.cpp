#include "globals.h"
#include "hook.h"

static char __fastcall ConsumeFlagByte26(void* self, void* /*edx*/)
{
    const char previousValue = CSF_AT(char, self, 0x26);
    if (previousValue)
        CSF_AT(char, self, 0x26) = 0;
    return previousValue;
}

HOOK(0x6B6BD0, ConsumeFlagByte26, "auto");
