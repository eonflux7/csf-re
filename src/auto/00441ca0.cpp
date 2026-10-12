#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

// Entry table: 16-byte records at field_0014 + 16*index.
// Record +0: pointer to a dword array of values; +8: element count.
// value == -1: returns the array pointer of the entry (0 if the entry is empty).
// otherwise: returns the address of the first array element equal to value (0 if none).
static int __fastcall DynamicObj_FindValueAddress(S_00441ca0* self, void* /*edx*/, int entryIndex, int value)
{
    int found = 0;

    if (value != -1)
    {
        GetValue8B0554();

        uint32_t entryAddr = (static_cast<uint32_t>(entryIndex) << 4) + self->field_0014;
        uint32_t count = CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 8);

        uint32_t i = 0;
        while (i < count)
        {
            if (found != 0)
                break;

            uint32_t arrayBase = CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 0);
            uint32_t elemAddr = arrayBase + 4u * i;
            if (CSF_AT(int, reinterpret_cast<void*>(elemAddr), 0) == value)
                found = static_cast<int>(elemAddr);
            ++i;
        }
        return found;
    }

    uint32_t entryAddr = (static_cast<uint32_t>(entryIndex) << 4) + self->field_0014;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 8) == 0)
        return found;
    return static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(entryAddr), 0));
}

HOOK(0x441CA0, DynamicObj_FindValueAddress, "auto");
