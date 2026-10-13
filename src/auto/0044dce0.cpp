#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: C_0082e664::FindLowerBoundIndex (0x41E660), thiscall, 1 stack arg.
CSF_FUNC(int, __fastcall, C_0082e664_FindLowerBoundIndex, 0x0041E660,
         (C_0082e664* self, void* /*edx*/, uint32_t key));

// Original: DynArray insert at index (0x41E510), thiscall on the embedded array at this+4, 2 stack args.
CSF_FUNC(int, __fastcall, DynArray_InsertAt, 0x0041E510,
         (void* self, void* /*edx*/, unsigned int index, uint32_t* value));

// Original: S_0041e5f0 grow capacity (0x41E5F0), thiscall, 1 stack arg.
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// The original references S_0041e5f0's vtable (CSF_VTBL(0x0041E5F0)) at the call site 0x44DD21.
// Bound here so the address appears in the file; it performs no store.
[[maybe_unused]] static const auto kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

// Stores a value in the sorted array at this+4, keeping it ordered.
// Returns the lower-bound index where the value was placed.
static unsigned int __fastcall C_0082e664_StoreAllocatedSlot(C_0082e664* self, void* /*edx*/, unsigned int a2)
{
    unsigned int value = a2;
    uint32_t lowerBoundIndex = C_0082e664_FindLowerBoundIndex(self, nullptr, value);

    if (lowerBoundIndex < self->field_000c)
    {
        // Existing element: insert in the middle of the array.
        DynArray_InsertAt(reinterpret_cast<void*>(&self->field_0004), nullptr,
                          lowerBoundIndex, &value);
        return lowerBoundIndex;
    }

    // Append at the end.
    uint32_t count = self->field_000c;
    uint32_t capacity = self->field_0008;
    auto* slots = reinterpret_cast<S_0041e5f0*>(&self->field_0004);

    if (count >= capacity)
    {
        S_0041e5f0_GrowArrayCapacity(slots, nullptr, static_cast<int>(slots->field_000c));
    }

    uint32_t slotIndex = slots->field_0008;
    uint32_t* slotPtr = reinterpret_cast<uint32_t*>(slots->field_0000 + 4 * slotIndex);
    slots->field_0008 = slotIndex + 1;
    *slotPtr = value;
    return lowerBoundIndex;
}

HOOK(0x0044DCE0, C_0082e664_StoreAllocatedSlot, "auto");
