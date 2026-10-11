#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00840f48_FindParticleEntry(
    C_00840f48* self, void* /*edx*/, int particle)
{
    const uint32_t count = self->field_0014;
    uint32_t offset = 0;

    for (uint32_t index = 0; index < count; ++index, offset += 16)
    {
        const auto* entries =
            reinterpret_cast<const uint8_t*>(self->field_000c);
        if (CSF_AT(uint32_t, entries + offset, 0x8) ==
            CSF_AT(uint32_t, reinterpret_cast<const void*>(particle), 0x4))
        {
            return static_cast<int>(index);
        }
    }

    return -1;
}

HOOK(0x0051E550, C_00840f48_FindParticleEntry, "auto");
