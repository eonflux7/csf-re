#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Pointer to the entry table used to look up per-entry headers/objects.
CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

// Returns 8 when there is no entry at (table + entryOffset + 4),
// 32 when the entry has no secondary object (entry + 4),
// otherwise 32 + the dword stored at (secondary + 0x14).
static int __cdecl GetEntrySizeWithHeader32(int entryOffset)
{
    void* entry = CSF_AT(
        void*, g_EntryTable, static_cast<std::uint32_t>(entryOffset) + 4u);
    if (!entry)
        return 8;

    void* secondary = CSF_AT(void*, entry, 4);
    if (!secondary)
        return 32;

    return static_cast<int>(CSF_AT(std::uint32_t, secondary, 0x14) + 0x20u);
}

HOOK(0x006B8DE0, GetEntrySizeWithHeader32, "auto");
