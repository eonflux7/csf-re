#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (copied from the packet).
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, ApplySceneEntryValue, 0x006B1840, (void* self, void* edx, int a2));

// Applies the pending scene-entry value stored at +0x24 (if any), then clears it.
// Returns the result of ApplySceneEntryValue, or 0 when nothing was pending.
static int __fastcall RpTeam_FlushPendingSceneEntry(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x24);
    if (result != 0)
    {
        int pendingValue = CSF_AT(int, self, 0x24);
        int globalInt = GetGlobalInt();
        result = static_cast<int>(ApplySceneEntryValue(reinterpret_cast<void*>(static_cast<intptr_t>(globalInt)), nullptr, pendingValue));
        CSF_AT(int, self, 0x24) = 0;
    }
    return result;
}

HOOK(0x006FF910, RpTeam_FlushPendingSceneEntry, "auto");
