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

CSF_FUNC(int, __fastcall, Object_SetIndexedValue, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Subobject_SetKeyedValue, 0x006C4BF0, (void* self, void* edx, int a2, unsigned int a3));

// Walks the 12-byte entry table of the holder object at +8 and applies each
// (key, value) pair to this object. Negative keys (sign bit set) are routed to
// the keyed-subobject setter with the sign bit cleared.
static unsigned int __fastcall C_008670f0_ApplyKeyValueEntries(C_008670f0* self, void* /*edx*/, unsigned int entryHolder)
{
    unsigned int result = entryHolder;
    uint32_t* entryTable = reinterpret_cast<uint32_t*>(
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(entryHolder), 0x8));

    if (entryTable)
    {
        result = entryTable[2];
        unsigned int entryIndex = 0;
        if (result)
        {
            int byteOffset = 0;
            while (true)
            {
                uint8_t* entries = reinterpret_cast<uint8_t*>(entryTable[0]);
                int key = *reinterpret_cast<int*>(entries + byteOffset);
                uint32_t value = *reinterpret_cast<uint32_t*>(entries + byteOffset + 4);

                if (key >= 0)
                    Object_SetIndexedValue(self, nullptr, key, static_cast<int>(value));
                else
                    Subobject_SetKeyedValue(self, nullptr, key & 0x7FFFFFFF, value);

                result = entryTable[2];
                ++entryIndex;
                byteOffset += 12;
                if (entryIndex >= result)
                    break;
            }
        }
    }
    return result;
}

HOOK(0x006C4CF0, C_008670f0_ApplyKeyValueEntries, "auto");
