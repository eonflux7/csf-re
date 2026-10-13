#include <cstdint>
#include "hook.h"

// Copies 8-byte (two-dword) records from the source range ending at a2 down to
// a stop address a1, writing them into a destination that grows downward from a3.
// Returns the destination pointer after the last record written.
static uint32_t* __cdecl CopyDwordPairsBackward(int32_t stopAddress, int32_t sourceEnd, uint32_t* destEnd) {
    int32_t cursor = sourceEnd;
    uint32_t* result = destEnd;
    while (cursor != stopAddress) {
        uint32_t lowDword = *reinterpret_cast<uint32_t*>(cursor - 8);
        cursor -= 8;
        result -= 2;
        result[0] = lowDword;
        result[1] = *reinterpret_cast<uint32_t*>(cursor + 4);
    }
    return result;
}

HOOK(0x004DC700, CopyDwordPairsBackward, "auto");
