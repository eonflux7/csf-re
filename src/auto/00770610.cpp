#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binary search over a sorted array of 104-byte sound-area records.
// The record key is a uint32_t at offset 0. Returns the record index, or -1.
static int __fastcall S_004ef780_FindEntryIndexById(S_004ef780* self, void* /*edx*/, unsigned int searchId)
{
    if (searchId == 0xFFFFFFFFu)
        return -1;

    // Element count = (end - begin) / 104, signed division. Empty when begin is null.
    uint32_t entriesBegin = self->field_0008;
    int entryCount = 0;
    if (entriesBegin)
        entryCount = (int)(self->field_000c - entriesBegin) / 104;

    int lo = 0;
    int hi = entryCount - 1;
    if (hi < 0)
        return -1;

    uint8_t* base = reinterpret_cast<uint8_t*>(entriesBegin);
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        uint32_t key = *reinterpret_cast<uint32_t*>(base + mid * 104);
        if (searchId <= key)
        {
            if (searchId >= key)
                return mid;
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;
        }
    }
    return -1;
}

HOOK(0x00770610, S_004ef780_FindEntryIndexById, "auto");
