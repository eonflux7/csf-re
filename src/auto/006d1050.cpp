#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxProps_ReadPropertyTableEntry, 0x006B88C0, (int a1));
CSF_FUNC(char, __fastcall, C_00861988_ApplyParameters, 0x006FD0E0, (void* self, void* edx));

// Walks the two property-id tables stored in the block: a1[0] is the count of
// the first table (ids at a1[2..]), a1[1] is the count of the second table
// (ids at a1[10..]). Each non-zero id is resolved to a property-table entry and
// its parameters are applied. Returns the second count as re-read after the loops.
static uint32_t __stdcall GfxLights_ApplyPropertyEntries(uint32_t* a1)
{
    uint32_t i = 0;
    if (a1[0] != 0)
    {
        uint32_t* slot = a1 + 2;
        do
        {
            if (*slot != 0)
            {
                C_00861988* entry = reinterpret_cast<C_00861988*>(
                    GfxProps_ReadPropertyTableEntry(static_cast<int>(*slot)));
                C_00861988_ApplyParameters(entry, nullptr);
            }
            ++i;
            ++slot;
        } while (i < a1[0]);
    }

    uint32_t result = a1[1];
    uint32_t j = 0;
    if (result != 0)
    {
        uint32_t* slot = a1 + 10;
        do
        {
            if (*slot != 0)
            {
                C_00861988* entry = reinterpret_cast<C_00861988*>(
                    GfxProps_ReadPropertyTableEntry(static_cast<int>(*slot)));
                C_00861988_ApplyParameters(entry, nullptr);
            }
            result = a1[1];
            ++j;
            ++slot;
        } while (j < result);
    }
    return result;
}

HOOK(0x006D1050, GfxLights_ApplyPropertyEntries, "auto");
