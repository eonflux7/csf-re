#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up an entry in the table held at this->field_0008 by key (key | 0x80000000).
// Table layout: [0] = entry array base, [8] = entry count. Each entry is 12 bytes:
// dword 0 = key, dwords 1-2 = payload. Returns a pointer to the payload (entry + 4) or 0.
static int __fastcall C_008670f0_FindEntryPayloadByKey(C_008670f0* self, void* /*edx*/, int key)
{
    uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0008);
    if (table == nullptr)
        return 0;

    uint32_t entryCount = CSF_AT(uint32_t, table, 0x8);
    if (entryCount == 0)
        return 0;

    uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, table, 0x0));
    const uint32_t wantedKey = static_cast<uint32_t>(key) | 0x80000000u;

    uint32_t index = 0;
    while (true) {
        if (entries[3 * index] == wantedKey)
            return static_cast<int>(reinterpret_cast<uintptr_t>(&entries[3 * index + 1]));
        ++index;
        if (index >= entryCount)
            return 0;
    }
}

HOOK(0x006C4910, C_008670f0_FindEntryPayloadByKey, "auto");
