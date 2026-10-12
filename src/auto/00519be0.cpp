#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

static int __fastcall BddTables_GetEntryElementCount(void* self, void* /*edx*/, int index)
{
    uint32_t table = CSF_AT(uint32_t, self, 0x0C);
    uint32_t entryOffset = static_cast<uint32_t>(index) * 16u;
    void* entry = reinterpret_cast<void*>(table + entryOffset);

    uint32_t begin = CSF_AT(uint32_t, entry, 0x04);
    if (begin)
    {
        uint32_t end = CSF_AT(uint32_t, entry, 0x08);
        return static_cast<int32_t>(end - begin) >> 2;
    }
    return 0;
}

HOOK(0x519BE0, BddTables_GetEntryElementCount, "auto");
