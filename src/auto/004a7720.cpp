#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_GetLookedUpTaskAtFieldOffset(C_00834fa8* self, void* /*edx*/) {
    // Original passes the same `this` (still in ecx) and (field at +0x0C) + 0x78 to the lookup.
    int lookupOffset = CSF_AT(int, self, 0x0C) + 0x78;
    return CharTask_LookupByOffset(self, nullptr, lookupOffset);
}

HOOK(0x004A7720, C_00834fa8_GetLookedUpTaskAtFieldOffset, "auto");
