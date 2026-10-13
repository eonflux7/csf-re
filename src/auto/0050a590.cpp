#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x0050A360: __fastcall(self, edx, a2) with ret 4 (one stack arg).
CSF_FUNC(char, __fastcall, OptionsFile_ProcessEntry, 0x0050A360, (void* self, void* edx, void* a2));

// Original: __fastcall-style (this in ecx, edx unused), two stack args, ret 8.
// Returns the callee's result when a2 is non-null and a3 == 8, otherwise 0.
static char __fastcall OptionsFile_HandleEntryOfSize8(S_0050a360* self, void* /*edx*/, int a2, int a3)
{
    // Vtable reference listed in the packet for this function.
    (void)CSF_VTBL(0x0050A360);

    if (a2 && a3 == 8)
        return OptionsFile_ProcessEntry(self, nullptr, reinterpret_cast<void*>(a2));
    return 0;
}

HOOK(0x0050A590, OptionsFile_HandleEntryOfSize8, "auto");
