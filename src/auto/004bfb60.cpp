#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address).
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable referenced by the original's array helper (kept for binding completeness; no runtime store).
[[maybe_unused]] static const auto kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

// Resolves the owner object linked to a1 and returns (linked - 4), or 0 when no link exists.
static uintptr_t ResolveLinkedOwnerBase(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    return linked ? static_cast<uintptr_t>(linked - 4) : 0;
}

// Appends a zeroed 32-bit entry to the owner's growable array and returns the new element count.
static int __stdcall AiCore_AppendZeroedLinkedEntry(int a1)
{
    // Stage 1: apply to the sub-object at owner + 0x440.
    uintptr_t ownerBase = ResolveLinkedOwnerBase(a1);
    UnnamedApply_4DC180(reinterpret_cast<void*>(a1), nullptr,
                        static_cast<int>(ownerBase + 0x440));

    // Stage 2: append into the array embedded at owner + 0x50C.
    ownerBase = ResolveLinkedOwnerBase(a1);
    uint8_t* arrayObj = reinterpret_cast<uint8_t*>(ownerBase) + 0x50C;

    // Grow when count >= capacity (unsigned compare, per jb in the disassembly).
    if (CSF_AT(uint32_t, arrayObj, 0x8) >= CSF_AT(uint32_t, arrayObj, 0x4))
    {
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(arrayObj), nullptr,
                                     static_cast<int>(CSF_AT(uint32_t, arrayObj, 0xC)));
    }

    // Reload the count after any growth, then append a zero slot.
    uint32_t count = CSF_AT(uint32_t, arrayObj, 0x8);
    uint32_t dataAddr = CSF_AT(uint32_t, arrayObj, 0x0);
    uint32_t* newSlot = reinterpret_cast<uint32_t*>(dataAddr + count * 4u);
    uint32_t newCount = count + 1;
    CSF_AT(uint32_t, arrayObj, 0x8) = newCount;
    *newSlot = 0;

    return static_cast<int>(newCount);
}

HOOK(0x004BFB60, AiCore_AppendZeroedLinkedEntry, "auto");
