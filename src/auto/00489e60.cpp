#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall UpdateActiveEntryProperties(void* self, void* /*edx*/,
                                                 int index, const void* source)
{
    const uint32_t offset = 96u * static_cast<uint32_t>(index);
    auto* entries = CSF_AT(uint8_t*, self, 0x0C);
    if (CSF_AT(uint32_t, entries + offset, 0x04))
    {
        CSF_AT(uint32_t, entries + offset, 0x38) =
            CSF_AT(uint32_t, source, 0x04);

        entries = CSF_AT(uint8_t*, self, 0x0C);
        CSF_AT(uint32_t, entries + offset, 0x40) =
            CSF_AT(uint32_t, source, 0x0C);
        CSF_AT(uint32_t, entries + offset, 0x44) =
            CSF_AT(uint32_t, source, 0x10);

        auto* destination = CSF_AT(uint8_t*, self, 0x0C) + offset + 0x48;
        CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, source, 0x14);
        CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, source, 0x18);
        CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, source, 0x1C);

        entries = CSF_AT(uint8_t*, self, 0x0C);
        CSF_AT(uint8_t, entries + offset, 0x58) =
            CSF_AT(uint8_t, source, 0x20);
        entries = CSF_AT(uint8_t*, self, 0x0C);
        CSF_AT(uint8_t, entries + offset, 0x5B) =
            CSF_AT(uint8_t, source, 0x21);
    }
    return static_cast<int32_t>(offset);
}

HOOK(0x00489E60, UpdateActiveEntryProperties, "auto");
