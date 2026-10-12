#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

// Original vtable of S_0041e5f0 (referenced by the call at 0x0044333D).
// No store is performed here; the original never writes this vtable in Reset.
[[maybe_unused]] static const auto s_GrowArrayVtbl = CSF_VTBL(0x0041E5F0);

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(int, __fastcall, sub_442F40, 0x00442F40, (void* self, void* edx));

// Resets the dynamic object's state: releases the active flag's resource if set,
// clears the cached pointer, and grows the internal array to at least 7 entries.
static char __fastcall C_00831074_Reset(C_00831074* self, void* /*edx*/)
{
    if (*reinterpret_cast<uint8_t*>(&self->field_0004) != 0)
    {
        sub_442F40(self, nullptr);
        *reinterpret_cast<uint8_t*>(&self->field_0004) = 0;
    }

    self->field_0008 = 0;

    uint32_t currentCapacity = CSF_AT(uint32_t, self, 0x10);
    if (currentCapacity < 7)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_000c),
            nullptr,
            static_cast<int>(7 - currentCapacity));
    }

    *reinterpret_cast<uint8_t*>(&self->field_0004) = 1;
    return 1;
}

HOOK(0x00443310, C_00831074_Reset, "auto");
