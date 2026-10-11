#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __fastcall, GrowEntryArray, 0x0048EB60,
               (void* self, void* edx, int a2));

static int __fastcall AppendEntryAndMarkDirty(void* self, void* /*edx*/,
                                             const uint32_t* entry)
{
    void* owner = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, self, 0x171) = 1;
    auto* array = static_cast<uint8_t*>(self) + 0xCC;
    CSF_AT(uint8_t, owner, 0x1D) = 1;

    if (CSF_AT(uint32_t, array, 0x08) >= CSF_AT(uint32_t, array, 0x04))
        GrowEntryArray(array, nullptr, CSF_AT(int, array, 0x0C));

    uint32_t count = CSF_AT(uint32_t, array, 0x08);
    auto* destination = CSF_AT(uint8_t*, array, 0x00) + 12 * count;
    CSF_AT(uint32_t, array, 0x08) = count + 1;

    CSF_AT(uint32_t, destination, 0x00) = entry[0];
    uint32_t result = entry[1];
    CSF_AT(uint32_t, destination, 0x04) = result;
    CSF_AT(uint32_t, destination, 0x08) = entry[2];
    return static_cast<int>(result);
}

HOOK(0x004906C0, AppendEntryAndMarkDirty, "auto");
