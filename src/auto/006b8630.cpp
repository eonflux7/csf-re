#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));

CSF_GLOBAL(uint32_t, g_PropsTableBase, 0x008BB484);
CSF_GLOBAL(uint32_t, g_RecordTag, 0x008A7E24);

// Writes a record of two 32-bit values to the RW stream: a global tag, then the
// first dword of the table entry at (g_PropsTableBase + tableOffset + 4).
// Returns the stream pointer.
static int GfxObjectProps_WriteTableEntryRecord(int stream, int /*unused*/, int tableOffset)
{
    uint32_t recordTag = g_RecordTag;
    RwStreamWriteInt32(reinterpret_cast<uint32_t*>(stream), &recordTag, 4);

    // Entry pointer lives at table base + offset + 4.
    uint32_t* entry = *reinterpret_cast<uint32_t**>(
        static_cast<uintptr_t>(g_PropsTableBase) + static_cast<uint32_t>(tableOffset) + 4);

    // Re-read the entry's first dword after the first write (original reloads it here).
    uint32_t entryValue = *entry;
    RwStreamWriteInt32(reinterpret_cast<uint32_t*>(stream), &entryValue, 4);

    return stream;
}

HOOK(0x006B8630, GfxObjectProps_WriteTableEntryRecord, "auto");
