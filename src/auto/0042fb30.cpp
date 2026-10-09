#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Resizes the slot array to newCount (growing it when needed) and fills every
// newly exposed slot [oldCount, newCount) with fillValue. Returns the final count.
static unsigned int __fastcall S_0041e5f0_ResizeAndFill(S_0041e5f0* self, void* /*edx*/, unsigned int newCount, int fillValue)
{
    uint32_t currentCapacity = self->field_0004;
    if (newCount > currentCapacity)
        GrowButtonArray(self, nullptr, (int)(newCount - currentCapacity));

    unsigned int index = self->field_0008;
    for (; index < newCount; ++index)
        reinterpret_cast<int*>(self->field_0000)[index] = fillValue;

    self->field_0008 = newCount;
    return index;
}

HOOK(0x42FB30, S_0041e5f0_ResizeAndFill, "auto");
