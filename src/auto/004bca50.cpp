#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Growable array embedded at +0x6C: data pointer at +0x6C, capacity at +0x70, count at +0x74.
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Appends action code 13 to the embedded array and returns 0 (byte return: only al is cleared).
static char __fastcall S_004bc930_PushActionCode13(S_004bc930* self, void* /*edx*/, int a2)
{
    // Referenced at the call site's target; the discarded use keeps the binding present.
    static_cast<void>(CSF_VTBL(0x0041E5F0));

    if (self->field_0074 >= CSF_AT(uint32_t, self, 0x70))
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x6C),
            nullptr,
            CSF_AT(int, self, 0x78));
    }

    // Re-read count and data pointer after a possible grow.
    uint32_t count = self->field_0074;
    uint32_t* slot = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x6C)) + 4u * count);
    self->field_0074 = count + 1;
    *slot = 13;
    return 0;
}

HOOK(0x004BCA50, S_004bc930_PushActionCode13, "auto");
