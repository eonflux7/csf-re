#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_00445eb0_FindMatchingEntry(
    S_00445eb0* self, void* /*edx*/, int descriptorAddress, int key)
{
    const auto* descriptor = reinterpret_cast<const uint8_t*>(descriptorAddress);
    if (CSF_AT(int32_t, descriptor, 0x08) < 0)
        return -1;

    const uint32_t count = self->field_0024;
    if (count == 0)
        return -1;

    const auto* entry = reinterpret_cast<const uint8_t*>(self->field_001c);
    for (uint32_t index = 0; index < count; ++index, entry += 0x28)
    {
        if (CSF_AT(int32_t, entry, 0x00) == key &&
            CSF_AT(uint32_t, entry, 0x0C) == CSF_AT(uint32_t, descriptor, 0x04))
            return static_cast<int32_t>(index);
    }
    return -1;
}

HOOK(0x00445EB0, S_00445eb0_FindMatchingEntry, "auto");
