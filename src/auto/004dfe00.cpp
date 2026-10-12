#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sift-down helper bound by address (ret 0, __cdecl).
CSF_FUNC(int, __cdecl, sub_4DF740, 0x004DF740, (int a1, int a2, int a3, int a4, float a5, int a6));

// Removes the top entry of a heap-like array of 8-byte {int key; float weight} records.
// self: [+4] = begin, [+8] = end of the record array.
static int __fastcall HeapPopTopKey(char** self, void* /*edx*/) {
    uint32_t beginAddr = CSF_AT(uint32_t, self, 0x4);
    uint32_t endAddr = CSF_AT(uint32_t, self, 0x8);
    int topKey = CSF_AT(int, reinterpret_cast<uint8_t*>(beginAddr), 0);

    if (static_cast<int>((endAddr - beginAddr) & 0xFFFFFFF8u) > 8) {
        uint8_t* endPtr = reinterpret_cast<uint8_t*>(endAddr);
        int lastKey = CSF_AT(int, endPtr, -8);
        float lastWeight = CSF_AT(float, endPtr, -4);

        // Move the top record into the last slot.
        uint32_t lastSlotAddr = endAddr - 8;
        uint8_t* lastSlot = reinterpret_cast<uint8_t*>(lastSlotAddr);
        CSF_AT(int, lastSlot, 0) = CSF_AT(int, reinterpret_cast<uint8_t*>(beginAddr), 0);
        CSF_AT(int, lastSlot, 4) = CSF_AT(int, reinterpret_cast<uint8_t*>(beginAddr), 4);

        int recordCount = static_cast<int>(lastSlotAddr - beginAddr) >> 3;
        sub_4DF740(static_cast<int>(beginAddr), 0, recordCount, lastKey, lastWeight, 0);
    }

    uint32_t curBegin = CSF_AT(uint32_t, self, 0x4);
    if (curBegin == 0)
        return topKey;

    uint32_t curEnd = CSF_AT(uint32_t, self, 0x8);
    if (static_cast<int>(curEnd - curBegin) >> 3 != 0)
        CSF_AT(uint32_t, self, 0x8) = curEnd - 8;

    return topKey;
}

HOOK(0x004DFE00, HeapPopTopKey, "auto");
