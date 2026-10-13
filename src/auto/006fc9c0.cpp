#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Searches the int array at +0x44 for `value`. If found, returns (index + 1).
// Otherwise appends `value` (growing the array first if full) and returns the new count.
static int __fastcall C_00862e88_FindOrAppendValue(C_00862e88* self, void* /*edx*/, int value)
{
    const int count = static_cast<int>(self->field_004c);

    if (count > 0) {
        int* items = reinterpret_cast<int*>(self->field_0044);
        for (int i = 0; i < count; ++i) {
            if (items[i] == value) {
                return i + 1;
            }
        }
    }

    // Not found: append.
    if (self->field_004c >= CSF_AT(uint32_t, self, 0x48)) {
        static_cast<void>(CSF_VTBL(0x0041E5F0));
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0044), nullptr,
                                     static_cast<int>(CSF_AT(uint32_t, self, 0x50)));
    }

    const uint32_t index = self->field_004c;
    int* items = reinterpret_cast<int*>(self->field_0044);
    items[index] = value;
    self->field_004c = index + 1;
    return static_cast<int>(index + 1);
}

HOOK(0x006FC9C0, C_00862e88_FindOrAppendValue, "auto");
