#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static bool __fastcall HasDifferentMatchingEntry(void* self, void* /*edx*/,
                                                const void* reference,
                                                const void* key)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x08);
    if (count == 0)
        return false;

    auto* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x00));
    auto* candidate = entries + 0x10;
    uint32_t index = 0;

    while (CSF_AT(int32_t, candidate, 0x04) < 0 ||
           CSF_AT(uint32_t, candidate, 0x00) != CSF_AT(uint32_t, key, 0x04))
    {
        ++index;
        candidate += 0x18;
        if (index >= count)
            return false;
    }

    if (index == UINT32_MAX)
        return false;

    return CSF_AT(uint32_t, entries + 24 * index, 0x04) !=
           CSF_AT(uint32_t, reference, 0x04);
}

HOOK(0x004485A0, HasDifferentMatchingEntry, "auto");
