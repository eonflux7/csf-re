#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Returns 1 if any 12-byte entry in the array at self+8..self+0xC has
// key-field (+4) equal to the key stored at entry+4, otherwise 0.
// self+8 = begin, self+0xC = end, element stride = 12.
static int __fastcall Global_ContainsEntryKey(uint32_t* self, void* /*edx*/, int a2)
{
    uint32_t begin = CSF_AT(uint32_t, self, 0x8);
    uint32_t count = begin;
    if (count)
    {
        int32_t byteSpan = (int32_t)(CSF_AT(uint32_t, self, 0xC) - begin);
        count = (uint32_t)(byteSpan / 12);
    }

    uint32_t index = 0;
    if (count)
    {
        uint32_t searchedKey = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a2), 0x4);
        uint8_t* cursor = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8)) + 4;
        do
        {
            if (searchedKey == CSF_AT(uint32_t, cursor, 0))
                break;
            ++index;
            cursor += 12;
        } while (index < count);
    }

    return index < count ? 1 : 0;
}

HOOK(0x0044AF70, Global_ContainsEntryKey, "auto");
