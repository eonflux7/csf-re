#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Appends firstValue to the array at +0x00 and secondValue to the array at +0x10
// (both use the same count/capacity/growth header layout). Returns the new count of the second array.
static int __fastcall S_0041e5f0_AppendTwoParallelValues(S_0041e5f0* self, void* /*edx*/, int firstValue, int secondValue)
{
    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(self, nullptr, self->field_000c);

    uint32_t firstIndex = self->field_0008;
    int* firstSlot = reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_0000 + 4 * firstIndex));
    self->field_0008 = firstIndex + 1;
    *firstSlot = firstValue;

    S_0041e5f0* second = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (second->field_0008 >= second->field_0004)
        S_0041e5f0_GrowArrayCapacity(second, nullptr, second->field_000c);

    uint32_t secondIndex = second->field_0008;
    int* secondSlot = reinterpret_cast<int*>(static_cast<uintptr_t>(second->field_0000 + 4 * secondIndex));
    int newCount = static_cast<int>(secondIndex + 1);
    second->field_0008 = static_cast<uint32_t>(newCount);
    *secondSlot = secondValue;
    return newCount;
}

HOOK(0x004BC810, S_0041e5f0_AppendTwoParallelValues, "auto");
