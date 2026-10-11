#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-slot worker: __thiscall-style callee (this in ecx, slot pointer and a3 on the stack, ret 8).
CSF_FUNC(uint32_t*, __fastcall, sub_6C2460, 0x006C2460, (void* self, void* edx, uint32_t* a2, int a3));

// Walks the eight-entry slot table at `slots`. For each non-null entry, calls the worker with
// `ownerObject` as this and `a3` as the extra argument. Returns the last value assigned to result.
static int __stdcall HAnim_ProcessSlotArray(uint32_t* slots, uint32_t* ownerObject, int a3)
{
    uint32_t result = reinterpret_cast<uintptr_t>(ownerObject);
    if (ownerObject)
    {
        for (int slotIndex = 0; slotIndex < 8; ++slotIndex)
        {
            result = slots[slotIndex];
            if (result)
            {
                result = reinterpret_cast<uintptr_t>(
                    sub_6C2460(reinterpret_cast<void*>(ownerObject), nullptr,
                               reinterpret_cast<uint32_t*>(result), a3));
            }
        }
    }
    return static_cast<int>(result);
}

HOOK(0x006E85B0, HAnim_ProcessSlotArray, "auto");
