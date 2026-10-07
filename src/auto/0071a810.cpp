#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall C_00864508_HasMatchingEntry(
    C_00864508* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0048;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_004c - begin) >> 2)
        : 0u;

    for (uint32_t index = 0; index < count; ++index) {
        // Reload the array base: a virtual call may change it.
        void* entry = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0048),
                   index * 4u));

        if (entry &&
            ((uint8_t(__fastcall*)(void*, void*))
                 CSF_AT(void**, entry, 0)[12])(entry, nullptr)) {
            return 1;
        }
    }

    return 0;
}

HOOK(0x0071A810, C_00864508_HasMatchingEntry, "auto");
