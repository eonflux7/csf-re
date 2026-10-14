#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamWriteInt32, 0x00629C70, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(bool, __cdecl, WriteEntryBodyToStream, 0x006B90B0, (uint32_t* a1, int a2, int a3, int a4, int a5));

CSF_GLOBAL(uint8_t*, g_EntryTable, 0x008BB480);
CSF_GLOBAL(uint32_t, g_EntryFormatVersion, 0x008A7E20);

// Writes the format version, a presence flag and, when the entry at the given
// table offset exists, its size field and body to the RW stream.
static uint32_t* __cdecl WriteObjectPropsEntryToStream(uint32_t* stream, int unusedA2, int entryOffset)
{
    (void)unusedA2;

    int* entry = *reinterpret_cast<int**>(g_EntryTable + entryOffset + 4);

    uint32_t formatVersion = g_EntryFormatVersion;
    RwStreamWriteInt32(stream, &formatVersion, 4);

    int presentFlag = (entry != nullptr) ? 1 : 0;
    RwStreamWriteInt32(stream, &presentFlag, 4);

    if (entry != nullptr)
    {
        int entrySize = entry[1];
        RwStreamWriteInt32(stream, &entrySize, 4);
        // The original leaves the stream, the address of entrySize and the size 4
        // (the RwStreamWriteInt32 argument) in the unused callee slots.
        WriteEntryBodyToStream(stream, entry[0], reinterpret_cast<int>(stream),
                               reinterpret_cast<int>(&entrySize), 4);
    }

    return stream;
}

HOOK(0x6B94A0, WriteObjectPropsEntryToStream, "auto");
