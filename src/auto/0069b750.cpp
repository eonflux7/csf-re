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

CSF_FUNC(int, __fastcall, ProbeTableEntryHandler, 0x00699350, (void* self, void* edx, unsigned int a2));

// __thiscall(this, a2, a3), ret 8. Picks a table row from the bit length of a3,
// asks the row's handler, and falls back to an inline slot inside the object.
static int __fastcall LookupIndexedTableEntry(uint8_t* self, void* /*edx*/, unsigned int a2, unsigned int a3)
{
    int tableIndex;
    if (a3 != 0)
    {
        unsigned int shifted = a3 >> 1;
        int bitCount = 0;
        while (shifted != 0)
        {
            shifted >>= 1;
            ++bitCount;
        }
        tableIndex = bitCount;
    }
    else
    {
        tableIndex = -1;
    }

    uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint32_t entryPtr = CSF_AT(uint32_t, table + 32 * tableIndex, 0);

    if (entryPtr != 0)
    {
        void* entry = reinterpret_cast<void*>(entryPtr);
        int result = ProbeTableEntryHandler(entry, nullptr, a2);
        if (result != 0)
            return result;
    }

    if (CSF_AT(uint32_t, self, 0x2C) < 8u)
        return static_cast<int>(reinterpret_cast<uintptr_t>(self + 0x18));
    return CSF_AT(int, self, 0x18);
}

HOOK(0x0069B750, LookupIndexedTableEntry, "auto");
