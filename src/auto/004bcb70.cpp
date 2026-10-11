#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable of the array helper, referenced by the original's grow call site.
[[maybe_unused]] static const auto kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

// Appends the action code 31 (0x1F) to the action array embedded at +0x6C of this object.
// Array header: data pointer at +0x6C, capacity at +0x70, count at +0x74 (field_0074),
// grow increment at +0x78.
static char __fastcall S_004bc930_PushActionThirtyOne(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* arrayHeader = reinterpret_cast<uint8_t*>(self) + 0x6c;

    if (self->field_0074 >= CSF_AT(uint32_t, self, 0x70)) {
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(arrayHeader), nullptr,
                                     CSF_AT(int, self, 0x78));
    }

    // Re-read the count and data pointer after a possible grow.
    uint32_t index = self->field_0074;
    uint32_t* slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x6c));
    uint32_t* slot = slots + index;
    self->field_0074 = index + 1;
    *slot = 31;
    return 1;
}

HOOK(0x004BCB70, S_004bc930_PushActionThirtyOne, "auto");
