#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry release routine (__fastcall, one implicit this, edx unused).
CSF_FUNC(char, __fastcall, WaypointGraph_ReleaseNodeEntry, 0x0075D430, (void* self, void* edx));

// Game heap free with debug info (__cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static constexpr std::size_t kNodeEntryStride = 0x48;

// Releases every node entry in the [begin, end) range (stride 0x48), frees the
// backing block if one exists, and clears the three list pointers.
static int __fastcall WaypointGraph_ResetNodeList(void* self, void* /*edx*/)
{
    uint8_t* cursor = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    if (cursor)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        for (; cursor != end; cursor += kNodeEntryStride)
        {
            WaypointGraph_ReleaseNodeEntry(cursor, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04))));
        }
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return 0;
}

HOOK(0x0075E070, WaypointGraph_ResetNodeList, "auto");
