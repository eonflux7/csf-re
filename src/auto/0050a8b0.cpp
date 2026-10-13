#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x0050A7C0: __fastcall(self, edx, a2) with ret 4 (one stack arg).
CSF_FUNC(char, __fastcall, OptionsFile_ProcessRecord, 0x0050A7C0, (void* self, void* edx, void* a2));

// Original: __thiscall (this in ecx, edx unused), two stack args, ret 8.
// Forwards a2 to the record processor when a2 is non-null and a3 == 8, otherwise returns 0.
static char __fastcall OptionsFile_TryProcessSizedEntry(S_0050a7c0* self, void* /*edx*/, int a2, int a3)
{
    // Vtable reference listed in the packet for this function.
    (void)CSF_VTBL(0x0050A7C0);

    if (a2 && a3 == 8)
        return OptionsFile_ProcessRecord(self, nullptr, reinterpret_cast<void*>(a2));
    return 0;
}

HOOK(0x0050A8B0, OptionsFile_TryProcessSizedEntry, "auto");
