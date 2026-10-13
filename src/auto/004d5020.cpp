#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (by original address)
CSF_FUNC(void*, __fastcall, sub_4D4B10, 0x004D4B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Releases the handle stored at +0x08 (via sub_4D4B10), then looks up the
// entry at (+0x0C + 0x50) through CharTask_LookupByOffset and returns its result.
static int __fastcall C_00834fa8_ReleaseHandleAndLookup(C_00834fa8* self, void* /*edx*/)
{
    sub_4D4B10(CSF_AT(void*, self, 0x8), nullptr);
    int lookupOffset = CSF_AT(int, self, 0xC) + 0x50;
    return CharTask_LookupByOffset(self, nullptr, lookupOffset);
}

HOOK(0x4D5020, C_00834fa8_ReleaseHandleAndLookup, "auto");
