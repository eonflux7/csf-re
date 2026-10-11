#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry release routine (__fastcall, one register argument in ecx).
CSF_FUNC(int, __fastcall, ResetWaterEntry, 0x0070CC60, (void* self, void* edx));

// Game heap free with debug info (__cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static constexpr std::size_t kWaterEntryStride = 0x8;

// Releases every water entry in the [begin, end) range (stride 8), frees the
// backing block if one exists, and clears the three list pointers.
static int __fastcall Water_ResetEntryList(void* self, void* /*edx*/)
{
    uint8_t* cursor = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)));
    if (cursor)
    {
        uint8_t* end = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
        for (; cursor != end; cursor += kWaterEntryStride)
        {
            ResetWaterEntry(cursor, nullptr);
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

HOOK(0x00709AE0, Water_ResetEntryList, "auto");
