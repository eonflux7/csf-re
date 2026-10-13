#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00861d48_FindEntryIndexById(C_00861d48* self, void* /*edx*/, int entryId)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x44);
    if (entryCount == 0)
        return -1;

    const uint8_t* entry = CSF_AT(uint8_t*, self, 0x3C);
    int index = 0;
    while (CSF_AT(int, entry, 0) != entryId)
    {
        ++index;
        entry += 0x14;
        if (static_cast<uint32_t>(index) >= entryCount)
            return -1;
    }
    return index;
}

HOOK(0x006E6D60, C_00861d48_FindEntryIndexById, "auto");
