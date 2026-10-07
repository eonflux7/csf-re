#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_EntryTable, 0x008BB480);

static int __cdecl GetEntrySizeWithHeader(int entryOffset)
{
    void* entry = CSF_AT(
        void*, g_EntryTable, static_cast<std::uint32_t>(entryOffset) + 4u);
    if (!entry)
        return 8;

    void* payload = CSF_AT(void*, entry, 0);
    if (!payload)
        return 16;

    return static_cast<int>(CSF_AT(std::uint32_t, payload, 0x14) + 16u);
}

HOOK(0x006B8E20, GetEntrySizeWithHeader, "auto");
