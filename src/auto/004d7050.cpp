#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_LookupTaskAtFieldOffset(C_00834fa8* self, void* /*edx*/)
{
    int lookupOffset = CSF_AT(int, self, 0x0C) + 84;
    return CharTask_LookupByOffset(self, nullptr, lookupOffset);
}

HOOK(0x004D7050, C_00834fa8_LookupTaskAtFieldOffset, "auto");
