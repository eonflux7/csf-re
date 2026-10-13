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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_ResetBase, 0x00748040, (void* self, void* edx));

// Refills the per-slot "selected index" tables with -1: one table always,
// a second one only when the flag byte at +0x6A is set. Returns the last
// loop counter of the second table (low byte), or 0 when the flag is clear.
static char __fastcall MpSpawnPoints_RefreshSelection(C_008310b8* self, void* /*edx*/)
{
    MpSpawnPoints_ResetBase(self, nullptr);

    const int slotCount = static_cast<int>(CSF_AT(int8_t, self, 0x69));
    CSF_AT(void*, self, 0x350) = Mem_Alloc(static_cast<std::size_t>(4 * slotCount));

    CSF_AT(int, self, 0x354) = 0;
    int counter = 0;
    while (counter < slotCount)
    {
        static_cast<int*>(CSF_AT(void*, self, 0x350))[counter] = -1;
        ++counter;
    }

    char result = 0;
    if (CSF_AT(uint8_t, self, 0x6A) != 0)
    {
        CSF_AT(void*, self, 0x358) = Mem_Alloc(static_cast<std::size_t>(4 * slotCount));

        CSF_AT(int, self, 0x35C) = 0;
        int secondCounter = 0;
        while (secondCounter < slotCount)
        {
            static_cast<int*>(CSF_AT(void*, self, 0x358))[secondCounter] = -1;
            ++secondCounter;
        }
        result = static_cast<char>(secondCounter);
    }

    return result;
}

HOOK(0x007480A0, MpSpawnPoints_RefreshSelection, "auto");
