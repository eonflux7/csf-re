#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Game heap allocator (__cdecl, one stack argument, caller pops).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Global table base pointer (dword_8BB488).
CSF_GLOBAL(uint32_t, g_EntryTableBase, 0x008BB488);

static int __cdecl GfxObjectProps_InitEntryRecord(int entryOffset)
{
    void* record = Mem_Alloc(12);
    if (record != nullptr)
    {
        CSF_AT(int, record, 0x0) = 0;
        CSF_AT(int, record, 0x4) = -1;
        CSF_AT(int, record, 0x8) = 0;
    }
    else
    {
        record = nullptr;
    }

    uint8_t* tableBase = reinterpret_cast<uint8_t*>(g_EntryTableBase);
    *reinterpret_cast<int*>(tableBase + entryOffset) = 0;
    *reinterpret_cast<void**>(tableBase + entryOffset + 4) = record;
    return entryOffset;
}

HOOK(0x006B8450, GfxObjectProps_InitEntryRecord, "auto");
