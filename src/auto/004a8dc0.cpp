#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004D05E0: __fastcall, takes (self in ecx, edx, targetHandle, positionIndex), ret 8.
CSF_FUNC(bool, __fastcall, C_00835228_IsTargetPositionClear, 0x004D05E0, (C_00835228* self, void* edx, int targetHandle, int positionIndex));

// Vtable entry with one stack argument (a1), ret 4. The original never writes ecx before
// calling 0x4D05E0, so the caller's ecx (this) is forwarded unchanged. It is therefore
// declared __fastcall with ecx as the first parameter; the single stack argument
// makes the callee pop 4 bytes, matching the original's `ret 4`.
static bool __fastcall C_00835228_IsTargetPositionClearForHandle(C_00835228* self, void* /*edx*/, C_00835228* a1)
{
    return C_00835228_IsTargetPositionClear(self, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(a1)), 20);
}

HOOK(0x004A8DC0, C_00835228_IsTargetPositionClearForHandle, "auto");
