#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall NetDdlObjectManager_FindAvailableSlot(
    char* self, void* /*edx*/)
{
    int bestIndex = -1;
    int32_t bestValue = -1;
    char* entry = self + 4;

    for (int index = 0; index < 30; ++index, entry += 0x30)
    {
        if (CSF_AT(uint8_t, entry, 0x28) != 0)
            continue;

        if (CSF_AT(int32_t, entry, 0x00) == -1)
            return index;

        if (CSF_AT(uint8_t, entry, 0x29) != 0)
            continue;

        const int32_t value = CSF_AT(int32_t, entry, 0x20);
        if (bestValue > value || bestValue == -1)
        {
            bestIndex = index;
            bestValue = value;
        }
    }

    return bestIndex;
}

HOOK(0x0074A250, NetDdlObjectManager_FindAvailableSlot, "auto");
