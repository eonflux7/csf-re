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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, Table_FindIndex, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Looks up the table index for a key taken from a nested record, then writes
// the number of 44-byte entries in that table record (as a float) into the
// output object at a1. Returns the pointer stored at a1+4 when the key is not found.
static int __stdcall CountEntriesForKey(int a1, int /*a2*/, int a3)
{
    // Read the key record: entry = *a3; key = entry[1]
    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a3), 0));
    uint8_t* keyRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 4));
    int keyValue = CSF_AT(int, keyRecord, 4);

    // First registry lookup: find the table index for the key.
    uint8_t* registry = reinterpret_cast<uint8_t*>(GetValue8B0554());
    int tableIndex = Table_FindIndex(CSF_AT(void*, registry, 0x1FD8), nullptr, keyValue);

    uint8_t* outObj = reinterpret_cast<uint8_t*>(a1);

    if (tableIndex == -1)
    {
        int resultPtr = CSF_AT(int, outObj, 4);
        CSF_AT(int, reinterpret_cast<uint8_t*>(resultPtr), 4) = 0;
        return resultPtr;
    }

    // Second registry lookup: fetch the table record for that index.
    uint8_t* registry2 = reinterpret_cast<uint8_t*>(GetValue8B0554());
    uint8_t* record = reinterpret_cast<uint8_t*>(
        Table_GetRecord(CSF_AT(void*, registry2, 0x1FD8), nullptr, tableIndex));

    // Element count of the vector stored at record+0x38 (begin) / record+0x3C (end),
    // elements are 44 bytes each.
    int count = 0;
    uint32_t begin = CSF_AT(uint32_t, record, 0x38);
    if (begin != 0)
    {
        uint32_t end = CSF_AT(uint32_t, record, 0x3C);
        int32_t diff = static_cast<int32_t>(end - begin);
        count = diff / 44;
    }

    // Signed count converted to float; negative values are biased as unsigned.
    float value = static_cast<float>(count);
    if (count < 0)
        value += g_UnsignedIntegerBias;

    uint8_t* outRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, outObj, 4));
    CSF_AT(float, outRecord, 4) = value;
    return a1;
}

HOOK(0x0045BF30, CountEntriesForKey, "auto");
