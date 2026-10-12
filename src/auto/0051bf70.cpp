#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-record reset routine (__fastcall, one implicit this argument).
CSF_FUNC(int*, __fastcall, ResetRecord68Entry, 0x0051BEB0, (void* self, void* edx));

// Game heap free with debug info (__cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static constexpr std::size_t kRecord68Stride = 0x44;

// Resets every 0x44-byte record in the [begin, end) range, frees the backing
// block if one exists, and clears the three list pointers.
static int __fastcall BddTables_ClearRecord68List(void* self, void* /*edx*/)
{
    uint8_t* cursor = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    if (cursor)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        for (; cursor != end; cursor += kRecord68Stride)
        {
            ResetRecord68Entry(cursor, nullptr);
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

HOOK(0x0051BF70, BddTables_ClearRecord68List, "auto");
